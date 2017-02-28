/*
 * AsyncHandler.c
 *
 * Created: 2017-02-27 13:56:53
 *  Author: Robin Andersson
 */ 

#include "AsyncHandler.h"
#include "TinyTimber.h"
#include <avr/io.h>

void asyncPorting(PulseGenerator *self, int arg) {
	SYNC(self, porting, 0);
}

void asyncHold(GUI *self, int arg) {
	SYNC(self, hold, 0);
}

void asyncSaveState(GUI *self, int arg) {
	SYNC(self->currentPulse, saveState, 0)
}

void asyncUpdateLCD(GUI *self, int arg) {
	SYNC(self, updateLCD, 0);
}

void asyncChange(GUI *self, int arg) {
	SYNC(self, change, arg);
}