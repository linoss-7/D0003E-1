/*
 * GUI.c
 *
 * Created: 2017-02-21 14:02:40
 *  Author: Robin Andersson
 */ 
#include "PulseGenerator.h"
#include "GUI.h"
#include "LCD.h"
#include "TinyTimber.h"
#include "AsyncHandler.h"
#include <avr/io.h>
#include <avr/iom169p.h>
#include <avr/interrupt.h>
#include <avr/portpins.h>

int changePortB(GUI *self, int arg) {	
	self->ah->firstPress = 1;
	
	if (((PINB >> 7) & 1) == 0) {									// down
		ASYNC(self->ah, hold, 0);
	} else if (((PINB >> 6) & 1) == 0) {							// up
		ASYNC(self->ah, hold, 0);
	}  else if (((PINB >> 4) & 1) == 0) {							// depressed
		ASYNC(self->currentPulse, saveState, 0);
		ASYNC(self->lcd, updateLCD, 0);
	}
	return 0;
}

int changePortE(GUI *self, int arg) {
	if (((PINE >> 3) & 1) == 0) {
		self->currentPulse = self->g2;
		ASYNC(self->lcd, change, 6);
	} else if (((PINE >> 2) & 1) == 0) {
		self->currentPulse = self->g1;
		ASYNC(self->lcd, change, 4);
	}
	self->ah->currentPulse = self->currentPulse;
	ASYNC(self->lcd, updateLCD, 0);
	return 0;
}



