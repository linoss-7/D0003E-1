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
#include <avr/io.h>

void updateLCD(GUI *self, int arg) {
	printAt(self->g1->frequency, 0);
	printAt(self->g2->frequency, 4);
}

void change(GUI *gui, int pulse) {
	if (pulse == 1) {
		LCDDR0 = LCDDR0 | 0x4;
		LCDDR1 = LCDDR1 ^ 0x2;
	} else if (pulse == 2) {
		LCDDR0 = LCDDR0 ^ 0x4;
		LCDDR1 = LCDDR1 | 0x2;
	}
}

int changePortB(GUI *self, int arg) {	
	if (PINB >> 7 == 0) {									// down
		ASYNC(self, asyncHold, 0);
	} else if (PINB >> 6 == 0) {							// up
		ASYNC(self, asyncHold, 0);
	}  else if (PINB >> 4 == 0) {							// depressed
		ASYNC(self, asyncSaveState, 0);
	}
	ASYNC(self, asyncUpdateLCD, 0);
	return 0;
}

int changePortE(GUI *self, int arg) {
	int pulse = 0;
	if (PINE >> 3 == 0) {
		self->currentPulse = self->g2;
		pulse = 2;
	} else if (PINE >> 2 == 0) {
		self->currentPulse = self->g1;
		pulse = 1;
	}
	ASYNC(self, asyncChange, pulse);
	ASYNC(self, asyncUpdateLCD, 0);
	return 0;
}

void hold(GUI *self, int arg) {
	if (self->currentPulse->firstPress) {
		self->currentPulse->firstPress = 0;
		AFTER(MSEC(1000), self, asyncHold, 0);
	}
	
	if (PINB >> 6 == 0) {
		SYNC(self->currentPulse, increasePulse, 0);
		SYNC(self, updateLCD, 0);
		AFTER(MSEC(500), self, asyncHold, 0);
	} else if (PINB >> 4 == 0) {
		SYNC(self->currentPulse, decreasePulse, 0);
		SYNC(self, updateLCD, 0);
		AFTER(MSEC(500), self, asyncHold, 0);
	}
}


