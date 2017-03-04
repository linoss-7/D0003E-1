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
	
	if (ch == 1) {
		LCDDR13 ^= 1;
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
		_SFR_MEM8(lcddr) = _SFR_MEM8(lcddr) | ((_SFR_MEM8(lcddr) & mask) | nibble);
		lcddr += 5;
	}
}

void printAt(long num, int pos) {
	int pp = pos;
	writeChar( (num % 100) / 10 + '0', pp);
	pp++;
	writeChar( num % 10 + '0', pp);
}

void init(int arg) {
	// disables clock prescaler
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// enables the logical interrupt sources
	EIMSK = 0xC0;
	EIFR = 0xC0;
	PCMSK1 |= (1<<PCINT15) | (1<<PCINT14) | (1<<PCINT12);
	PCMSK0 |= (1<<PCINT2) | (1<<PCINT3);
	
	PORTB |= 0xD0;			
	PORTE |= (1<<PE2) |(1<<PE3);	
	
	// Enable the LCD see page 218 set low-power waveform see page 219 set no frame interrupt set no blanking
	LCDCRA = (1<<LCDEN)|(1<<LCDAB)|(0<<LCDIE)|(0<<LCDBL);
	
	// set the prescaler setting to 16 see page 221 et the clock divider to 8 see page 220
	LCDFRR = (1<<LCDCD2)|(1<<LCDCD1)|(1<<LCDCD0)|(0<<LCDPS2)|(0<<LCDPS1)|(0<<LCDPS0);
	
	// set bias to 1/3 and duty to 1/4 see page 219  LCDCS is control signal if the it is clocked an external asynchronous clock scource see page 210
	LCDCRB = (0<<LCD2B)|(1<<LCDMUX0)|(1<<LCDMUX1)|(1<<LCDCS)| (1<<LCDPM2) | (1<<LCDPM1)|(1<<LCDPM0);
	
	//LCDCC3-0 decides the voltage, set voltage to 3,35 V see page 223 Set the normal drive time to 300 * 10^-6 s see page 222
	LCDCCR = (1<<LCDCC3) |(1<<LCDCC2) | (1<<LCDCC1)|(1<<LCDCC0)|(0<<LCDDC2)|(0<<LCDDC1)|(0<<LCDDC0);
	
	// reset timer
	TCNT1 = 0x0;

	// see page 117 set OC1A on compare match
	TCCR1A = (1<<COM1A1) | (1<< COM1A0);
	// set prescaler to 1024 and ctc
	TCCR1B = (1<<WGM12) | (1<<CS12) | (0 << CS11) | (1 << CS10);
	
	// Enable output compare A match interrupt
	TIMSK1 |= (1<<OCIE1A);
	
	// 8000000/ 1024 = 7812.5= 1sec, 7812.5/100 = 78.125, 78.125*5 = 390.625 ~ 391, the value the register has after 0.05 sec = 0x187
	OCR1A = 0x187;
	
	LCDDR0 |= 0x4; // init left pulse
	
	//TCCR1B = 0x0D;
}

void updateLCD(LCD *self, int arg) {
	int bitID = SYNC(self->currentPulse, getId, 0);
	if (bitID == 4) {
		printAt(self->currentPulse->frequency, 0);
	} else if (bitID == 6) {
		printAt(self->currentPulse->frequency, 4);
	}
	
}

void change(LCD *self, int pulse) {
	if (pulse == 4) {
		LCDDR0 |= 0x4;
		LCDDR1 &= 0xFD;
		self->currentPulse = self->g1;
	} else if (pulse == 6) {
		LCDDR0 &= 0xFB;
		LCDDR1 |= 0x2;
		self->currentPulse = self->g2;
	}
}
