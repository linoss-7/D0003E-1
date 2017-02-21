/*
 * GUI.c
 *
 * Created: 2017-02-21 14:02:40
 *  Author: Robin Andersson
 */ 
#include "PulseGenerator.h"
#include "GUI.h"
#include <avr/io.h>

void updateLCD(GUI *self) {
	if (self->currentPulse->bitNumber == 1) {
		printAt(self->currentPulse->frequency, 0);
	} else if (self->currentPulse->bitNumber == 2) {
		printAt(self->currentPulse->frequency, 4);
	}
}

void rightSwitch(GUI *self) {
	if (self->currentPulse->bitNumber == 1) {
		self->currentPulse = self->g2;
	}
}

void leftSwitch(GUI *self) {
	if (self->currentPulse->bitNumber == 2) {
		self->currentPulse = self->g1;
	}
}

void upSwitch(GUI *self) {
	increasePulse(self->currentPulse);
	updateLCD(self);
}

void downSwitch(GUI *self) {
	decreasePulse(self->currentPulse);
	updateLCD(self);
}

void depressed(GUI *self) {
	saveState(self->currentPulse);
}

void changePortB(GUI *self, int arg) {
	if (PINB >> 7 == 0) {
		downSwitch(self);
	} else if (PINB >> 6 == 0) {
		upSwitch(self);
	} 
}

void changePortE(GUI *self, int arg) {
	if (PINE >> 2 == 0) {
		leftSwitch(self);
	} else if (PINE >> 3 == 0) {
		rightSwitch(self);
	}
}
