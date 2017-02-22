/*
 * GUI.c
 *
 * Created: 2017-02-21 14:02:40
 *  Author: Robin Andersson
 */ 
#include "PulseGenerator.h"
#include "GUI.h"
#include <avr/io.h>

void updateLCD(GUI *self, int arg) {
	printAt(self->currentPulse.frequency, 0);
	printAt(self->currentPulse.frequency, 4);
}

void changePortB(GUI *self, int arg) {
	PulseGenerator* currentPulse = &(self->currentPulse);
	if (PINB >> 7 == 0) {									// down
		ASYNC(currentPulse, decreasePulse, 0);
		updateLCD(self, 0);
	} else if (PINB >> 6 == 0) {							// up
		ASYNC(currentPulse, increasePulse, 0);
		updateLCD(self, 0);
	}  else if (PINB >> 4 == 0) {							// depressed
		ASYNC(currentPulse, saveState, 0);
	}
}

void changePortE(GUI *self, int arg) {
	PulseGenerator* currentPulse = &(self->currentPulse);
	if (PINE >> 2 == 0) {
		if (currentPulse->bitNumber == 2) {
			self->currentPulse = self->g1;
		}
	} else if (PINE >> 3 == 0) {
		if (currentPulse->bitNumber == 1) {
			self->currentPulse = self->g2;
		}
	}
}
