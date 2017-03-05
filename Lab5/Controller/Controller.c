/*
 * Controller.c
 *
 * Created: 2017-03-05 13:32:43
 *  Author: Robin Andersson
 */ 

#include "Controller.h"
#include "LCD.h"
#include "TinyTimber.h"


void trafficLights(Controller *self, int arg) {
	if (self->northQueue == 0 && self->southQueue == 0) {
		SYNC(self, sendToSimulator, 0xA);
	} else if (self->northQueue > 0 && self->southQueue == 0) {
		SYNC(self, sendToSimulator, 0x9);
	} else if (self->northQueue == 0 && self->southQueue > 0) {
		SYNC(self, sendToSimulator, 0x6);
	} else if (self->previousQueue == 0) {
		self->previousQueue = 1;
		SYNC(self, sendToSimulator, 0x6);
	} else if (self->previousQueue == 1) {
		self->previousQueue = 0;
		SYNC(self, sendToSimulator, 0x9);
	}
}


void bitUSART(Controller *self, uint8_t data) {
	SYNC(self->lcd, updateSouth, data);
	
	if ((data & 1) == 1) {					// Northbound car arrival sensor activated
		self->northQueue++;
		SYNC(self, trafficLights, 0);
		SYNC(self, updateStatus, 0);
	} else if (((data >> 1) & 1) == 1) {	// Northbound bridge entry sensor activated
		if (self->northQueue > 0) {
			self->northQueue--;
		}
		SYNC(self, trafficLights, 0);
		SYNC(self, updateStatus, 0);
	} else if (((data >> 2) & 1) == 1) {	// Southbound car arrival sensor activated
		self->southQueue++;
		SYNC(self, trafficLights, 0);
		SYNC(self, updateStatus, 1);
	} else if (((data >> 23) & 1) == 1) {	// Southbound bridge entry sensor activated
		if (self->southQueue > 0) {
			self->southQueue--;
		}
		SYNC(self, trafficLights, 0);
		SYNC(self, updateStatus, 1);
	}
}

void sendToSimulator(Controller *self, uint8_t data) {
	SYNC(self->lcd, updateNorth, data);
	UDR0 = data;
}
