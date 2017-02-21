/*
 * Lab4.c
 *
 * Created: 2017-02-21 11:33:14
 * Author : Robin Andersson
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "PulseGenerator.h"
#include "GUI.h"
#include "TinyTimber.h"

void init(void) {
	// disables clock prescaler
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// enables the logical interrupt source PCINT15
	EIMSK = (1 << PCIE1);
	PCMSK1 = (1 << PCINT15);
	
	PORTB = 0xD0;			//PORTB = (1 << PORTB7)|(1 << PORTB6)|(1 << PORTB4);
	PORTE = 0x0c;			//PORTE = (1 << PORTE3)|(1 << PORTE2);
	
	// LCD enabled, low power waveform, no frame interrupt, no blanking
	LCDCRA = (1 << LCDEN) | (1 << LCDAB);
	// external asynchronous clock source, 1/3 bias, 1/4 duty cycle, 25 segments enabled
	LCDCRB = (1 << LCDCS) | (1 << LCDMUX0) | (1 << LCDMUX1) | (1 << LCDPM0) | (1 << LCDPM1) | (1 << LCDPM2);
	// prescaler setting N=16, clock divider setting D=8
	LCDFRR = (1 << LCDCD0) | (1 << LCDCD1) | (1 << LCDCD2);
	// drive time 300 microseconds, contrast control voltage 3.35 V
	LCDCCR = (1 << LCDCC0) | (1 << LCDCC1) | (1 << LCDCC2) | (1 << LCDCC3);
}

PulseGenerator g1 = initPulseGenerator(1);		//freq, pos, old, saved, gate, whichpulse
PulseGenerator g2 = initPulseGenerator(2);
GUI gui = initGUI(&g1, &g1, &g2);

int main(void) {
  init();
  
  INSTALL(&gui, changePortB, PCINT1_vect);
  INSTALL(&gui, changePortE, PCINT0_vect);
}

