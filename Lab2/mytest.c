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

int pp;

void LCD_Init(void) {
	// LCD enabled, low power waveform, no frame interrupt, no blanking
	LCDCRA = (1 << LCDEN) | (1 << LCDAB);
	// external asynchronous clock source, 1/3 bias, 1/4 duty cycle, 25 segments enabled
	LCDCRB = (1 << LCDCS) | (1 << LCDMUX0) | (1 << LCDMUX1) | (1 << LCDPM0) | (1 << LCDPM1) | (1 << LCDPM2);
	// prescaler setting N=16, clock divider setting D=8
	LCDFRR = (1 << LCDCD0) | (1 << LCDCD1) | (1 << LCDCD2);
	// drive time 300 microseconds, contrast control voltage 3.35 V
	LCDCCR = (1 << LCDCC0) | (1 << LCDCC1) | (1 << LCDCC2) | (1 << LCDCC3);
}

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
	// yield(); --Part 1--
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

// Defines interrupt handler for PCINT1_vect
ISR(PCINT1_vect) {
	if (PINB >> 7 == 0) { // maskes sure that the button is actually depressed
		yield();
	}
}

// Defines timer interrupt for TIMER1_COMPA_vect
ISR(TIMER1_COMPA_vect) {
	yield();
}

int main() {
	// disables clock prescaler
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// initializes the LCD
	LCD_Init();
	
	spawn(computePrimes, 0);
	computePrimes(3);
}