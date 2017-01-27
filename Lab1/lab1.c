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
	uint16_t SCCTable[10] =  {0x1551, 0x0110, 0x1e11, 0x1B11, 0x0B50, 0x1B41, 0x1F41, 0x0111, 0x1F51, 0x0B51};
	uint16_t sccChar = 0x0000;
	char nibble = 0x00;
	
	// checks if ch is in 0..9
	if (ch < 0 || ch > 9) {
		return;
	}
	
	// get the scc value from our scc table
	sccChar = SCCTable[(int) ch];
	
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
		
		if (pos % 2 != 0) {
			nibble = nibble << 4;
		}

		// värde på det lcddr pekar på
		_SFR_MEM8(lcddr) = (_SFR_MEM8(lcddr) & mask) | nibble;		
		lcddr += 5;
	}
}

void writeLong(long i) {
	int pos = 5;
	while (i > 0) {
		int ch = i % 10;
		writeChar(ch, pos);
		pos--;
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
	long i = 2;
	while (1) {
		if (is_prime(i)) {
			writeLong(i);
		}
		i++;
	}
}

void blink(void) {
	TCCR1B = (1 << CS12) | (0 << CS11) | (0 << CS10);
	uint16_t timerValue = 0xFFFF / 2;
	uint8_t check = 0;
	
	while (1) {
		if (TCNT1 >= timerValue && check == 0) {
			check == 1;
			if (LCDDR0 != 0) {
				LCDDR0 = 0x0;
			} else {
				LCDDR0 = 0x6;
			}
		}
		
		if (TCNT1 < timerValue) {
			check = 0;
		}
	}
}

void button(void) {
	PORTB = 0x80;
	uint8_t previousButton = 0;
	
	while (1) {
		if ( (PINB >> 7) == 1 && previousButton == 0) {
			LCDDR1 = 0x2;
			LCDDR2 = 0x0;
			previousButton = 1;
		} if ( (PINB >> 7) == 0  && previousButton == 1) {
			LCDDR2 = 0x4;
			LCDDR1 = 0x0;
			previousButton = 0;
		}
	}
}

void combine(long number) {
	PORTB = 0x80;
	TCCR1B = (1 << CS12) | (0 << CS11) | (0 << CS10);
	uint16_t timerValue = 0xFFFF / 2;
	uint8_t previousButton = 0;
	TCNT1 = 0x0000;
	uint8_t check = 0;
	
	while (1) {
	
		if ( (PINB >> 7) == 1 && previousButton == 0) {
			LCDDR18 = 0x1;
			LCDDR13 = 0x0;
			previousButton = 1;
		} if ( (PINB >> 7) == 0  && previousButton == 1) {
			LCDDR13 = 0x1;
			LCDDR18 = 0x0;
			previousButton = 0;
		}
		
		if (is_prime(number)) {
			writeLong(number);
		}
		
		if (TCNT1 >= timerValue && check == 0) {
			check = 1;
			if (LCDDR3 != 0) {
				LCDDR3 = 0x0;
				} else {
				LCDDR3 = 0x1;
			}
		}
		
		if (TCNT1 < timerValue) {
			check = 0;
		}
		
		number++;
		
	}
}

int main(void) {
	// disables clock prescaler
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// initializes the LCD
	LCD_Init();
	
	while (1) {
		//writeChar(3, 4);
		//writeLong(1234567);
		//primes()
		//blink()
		//button();
		combine(2);
    }
}
