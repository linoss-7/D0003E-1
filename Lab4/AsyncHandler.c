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

void hold(PulseGenerator *self, int arg) {
	/*if (self->firstPress) {
		self->firstPress = 0;
		AFTER(MSEC(1000), self, hold, 0);
	}*/
	
	
	
	if (((PINB >> 6) & 1) == 0) {
		SYNC(self, increasePulse, 0);
	} else if (((PINB >> 7) & 1) == 0) {
		SYNC(self, decreasePulse, 0);
	}
	
	SYNC(self, updateLCD, 0);
	AFTER(MSEC(500), self, hold, 0);
}