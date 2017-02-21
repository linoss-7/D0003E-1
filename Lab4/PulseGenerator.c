/*
 * PulseGenerator.c
 *
 * Created: 2017-02-21 11:49:19
 *  Author: Robin Andersson
 */ 

#include <avr/io.h>
#include <avr/iom169p.h>
#include "PulseGenerator.h"
	
void increasePulse(PulseGenerator *self, int arg) {
	if (self->frequency < 99) {
		if (self->frequency == 0) {
			self->frequency++;
			goToPort(self);
		} else {
			self->frequency++1;
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
		goToPort(self);
	}
}

void goToPort(PulseGenerator *self) {
	if (self->frequency > 0) {
		// set which port...
	}
}
