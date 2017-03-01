/*
 * PulseGenerator.c
 *
 * Created: 2017-02-21 11:49:19
 *  Author: Robin Andersson
 */ 

#include <avr/io.h>
#include <avr/iom169p.h>
#include <avr/portpins.h>
#include "PulseGenerator.h"
#include "Port.h"
	
void increasePulse(PulseGenerator *self, int arg) {
	LCDDR13 ^= 1;
	if (self->frequency < 99) {
		if (self->frequency == 0) {
			self->frequency++;
			goToPort(self, 0);
		} else {
			LCDDR3 ^= 1;
			self->frequency++;
		}
		
	}
}

void decreasePulse(PulseGenerator *self, int arg) {
	if (self->frequency > 0) {
		self->frequency--;
	}
}

void saveState(PulseGenerator *self, int arg) {
	if (self->saved == 0) {
		self->oldFrequency = self->frequency;
		self->frequency = 0;
		self->saved = 1;
	} else if (self->saved == 1) {
		self->frequency = self->oldFrequency;
		self->oldFrequency = 0;
		self->saved = 0;
		goToPort(self, 0);
	}
}

void goToPort(PulseGenerator *self, int arg) {
	if (self->frequency > 0) {
		SYNC(self, porting, 0);
		AFTER(MSEC(2000/(self->frequency)), self, porting, 0);
	}
}


