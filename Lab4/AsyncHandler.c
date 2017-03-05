/*
 * AsyncHandler.c
 *
 * Created: 2017-02-27 13:56:53
 *  Author: Robin Andersson
 */ 

#include "AsyncHandler.h"
#include "TinyTimber.h"
#include "PulseGenerator.h"
#include "LCD.h"
#include <avr/io.h>

void hold(AsyncHandler *self, int arg) {
	if (self->held == 0) {
		self->held = 1;
		AFTER(MSEC(500), self, hold, 0);
	} else if (((PINB >> 6) & 1) == 0) {
		SYNC(self->lcd->currentPulse, increasePulse, 0);
		SYNC(self->lcd, updateLCD, 0);
		AFTER(MSEC(200), self, hold, 0);
	} else if (((PINB >> 7) & 1) == 0) {
		SYNC(self->lcd->currentPulse, decreasePulse, 0);
		SYNC(self->lcd, updateLCD, 0);
		AFTER(MSEC(200), self, hold, 0);
	} else {
		self->held = 0;
	}
}