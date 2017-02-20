/*
*
* Lab 2
* Robin Andersson, roband-5
* Linus Arvidsson, linarv-4
*
*/

#include "tinythreads.h"
#include <avr/io.h>
#include <ctype.h>
#include <avr/interrupt.h>

int pp; // global variable
long buttonCount = 0;
mutex blinkMutex = MUTEX_INIT;
mutex buttonMutex = MUTEX_INIT;

void writeChar(char ch, int pos) {
	// Writes a char ch on position pos on the lcd
	char mask = 0x00;
	char lcddr = 0xEC;
	uint16_t SCCTable[10] =  {0x1551, 0x0110, 0x1e11, 0x1B11, 0x0B50, 0x1B41, 0x1F41, 0x0111, 0x1F51, 0x0B51};
	uint16_t sccChar = 0x0000;
	char nibble = 0x00;
	int number;
	
	// checks if ch is in 0..9
	if (isdigit(ch)) {
		number = ch - '0';
	} else if (ch <= 9 && ch >= 0) {
		number = ch;
	} else {
		return;
	}

	
	// get the scc value from our scc table
	sccChar = SCCTable[number];
	
	// decides which mask is used depending on if the desired position is even or not
	if (pos % 2 == 0) {
		mask = 0xF0;
	} else {
		mask = 0x0F;
	}
	
	// checks if the desired position is in 0..5
	if (pos < 0 || pos > 5) {
		return;
	}
	
	// 5, 4 >> LCDDR2 and 3, 2 >> LCDDR1 and 1, 0 >> LCDDR0
	lcddr += pos >> 1;
	
	// use our 4 nibbles to show the desired number at the desired position
	for (int i = 0; i < 4; i++) {
		nibble = sccChar & 0x000f;
		sccChar = sccChar >> 4;
		
		// shift nibble left 4 times if pos is uneven because then mask = 0x0F
		if (pos % 2 != 0) {
			nibble = nibble << 4;
		}

		// use the nibble and the mask to light the parts that nibble says
		_SFR_MEM8(lcddr) = (_SFR_MEM8(lcddr) & mask) | nibble;
		lcddr += 5;
	}
}


int is_prime(long i) {
	// checks if the long i is a prime
	for (int n = i-1; n >= 2; n--) {
		if (i % n == 0 && i != n) { // if the number is divisible by something else than itself and 1, return 0
			return 0;
		}
	}
	return 1; // if the number passses through the loop without returning 0, then it's a prime
}

void printAt(long num, int pos) {
	pp = pos;
	writeChar( (num % 100) / 10 + '0', pp);
	pp++;
	writeChar( num % 10 + '0', pp);
}

void computePrimes(int pos) {
	long n;
	
	for(n = 1; ; n++) {
		if (is_prime(n)) {
			printAt(n, pos);
		}
	}
}

void blink() {
	// makes a segment of the lcd blink
	while (1) {
		lock(&blinkMutex);
		if (LCDDR13 != 0) { // if the display is on and we've passed timerValue, turn it off
			LCDDR13 = 0x0;
		} else { // if the display is off and we've passed timerValue, turn it on
			LCDDR13 = 0x1;
		}
	}
}

void button(int pos) {
	while (1) {
		lock(&buttonMutex);
		printAt(buttonCount, pos);
	}
}

void timerInit(void) {
	TCCR1A = (1 << COM1A1) | (1 << COM1A0); // sets OC1A high on compare match
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // set the timer to Clear on Timer Compare and set timer prescaler factor to 1024
	OCR1A = 8000000/2048; // set a suitable value to OCR1A
	TCNT1 = 0x0; // clears the TCNT1 register
	TIMSK1 = (1 << OCIE1A); // enables timer output compare A interrupts
}


int main() {
	timerInit();
	
	spawn(blink, 0);
	spawn(button, 4);
	computePrimes(0);
}

ISR(PCINT1_vect){
	if((PINB >> 7) == 0) {
		buttonCount++;
		unlock(&buttonMutex);
		yield();
	}
}

ISR(TIMER1_COMPA_vect) {
	unlock(&blinkMutex);
	yield();
}