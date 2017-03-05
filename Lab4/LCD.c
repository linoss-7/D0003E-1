/*
 * LCD.c
 *
 * Created: 2017-02-21 14:31:16
 *  Author: Robin Andersson
 */

#include "LCD.h"
#include "PulseGenerator.h"
#include "TinyTimber.h"
#include <avr/io.h> 
#include <ctype.h>

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
		if (pos & 0x01) {
			nibble = nibble << 4;
		}

		// use the nibble and the mask to light the parts that nibble says
		_SFR_MEM8(lcddr) =  ((_SFR_MEM8(lcddr) & mask) | nibble);
		lcddr += 5;
	}
}

void printAt(long num, int pos) {
	int pp = pos;
	writeChar( (num % 100) / 10 + '0', pp);
	pp++;
	writeChar( num % 10 + '0', pp);
}

void init(void) {
	//Clock prescaler
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// enables the logical interrupt sources
	EIMSK = 0xC0;
	EIFR = 0xC0;
	PCMSK1 |= 0xD0;
	PCMSK0 |= 0x0C;
	
	PORTB |= 0xD0;			
	PORTE |= 0x0C;
	
	// LCD
	LCDCRA = 0xC0;			// LCD ENABLE and LOW POWER WAVEFORM
	LCDCRB = 0xB7;			// AsyncClock, 1/3 Bias, 1/4 Duty, 25 Segments
	LCDFRR = 0x07;			// LCD Clock Divide 32 Hz
	LCDCCR = 0x0F;			// 3.35 Volt
	
	LCDDR13 = 0x1;			// init left pulse
}

void updateLCD(LCD *self, int arg) {
	printAt(self->g1->frequency, 0);
	printAt(self->g2->frequency, 4);	
}

void change(LCD *self, int pulse) {
	if (pulse == 4) {
		LCDDR13 = 0x1;
		LCDDR18 = 0x0;
		self->currentPulse = self->g1;
	} else if (pulse == 6) {
		LCDDR13 = 0x0;
		LCDDR18 = 0x1;
		self->currentPulse = self->g2;
	}
}
