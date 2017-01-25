/*
 * Lab1.c
 *
 */ 

#include <avr/io.h>
#include <stdlib.h>


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
	char mask = 0x00;
	char lcddr = 0xEC;
	int SCCTable[10] =  {0x1551, 0x0110, 0x1e11, 0x1B11, 0x0B50, 0x1B41, 0x1F41, 0x0111, 0x1F51, 0x0B51};
	int sccChar = 0x0000;
	char nibble = 0x00;

	if (ch < 0 || ch > 9) {
		return;
	}

	sccChar = SCCTable[(int) ch];
	
	if (pos % 2 == 0) {
		mask = 0xF0;
	} else {
		mask = 0x0F;
	}

	if (pos < 0 || pos > 5) {
		return;
	}
	
	// startv�rde lcddr	

	for (int i = 0; i < 4; i++) {
		nibble = sccChar & 0x000f;
		sccChar = sccChar >> 4;
		
		if (pos  % 2 == 0) {
			nibble = nibble << 4;
		}

		// v�rde p� det lcddr pekar p�		
		lcddr += 5;
	}
}

void writeLong(long i) {
	int pos = 0;
	while (i > 0 || pos < 6) {
		int ch = i % 10;
		writeChar(ch, pos);
		pos++;
		i = i / 10;
	}
}

int is_prime(long i) {
	for (int n = i-1; n >= 2; n--) {
		if (i % n == 0 && i != n) {
			return 0;
		}
	}
	return 1;
}

void primes(void) {
	while (1) {
		long i = 2;
		if (is_prime(i)) {
			writeLong(i);
		}
		i++;
	}
}

int main(void) {
	// disables clock prescaler
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// initializes the LCD
	LCD_Init();
	
	while (1) {
		writeChar(1, 0);	
    }
}
