/*
 * Controller.c
 *
 * Created: 2017-03-05 13:32:43
 *  Author: Robin Andersson
 */ 

#include "Controller.h"
#include "LCD.h"
#include "TinyTimber.h"
#include "avr/io.h"



void trafficLights(Controller *self, int arg) {
	if (self->northQueue == 0 && self->southQueue == 0) {
		ASYNC(self, sendToSimulator, 0xA);
	} else if (self->northQueue > 0 && self->southQueue == 0) {
		ASYNC(self, sendToSimulator, 0x9);
	} else if (self->northQueue == 0 && self->southQueue > 0) {
		ASYNC(self, sendToSimulator, 0x6);
	} else if (self->previousQueue == 0) {
		self->previousQueue = 1;
		ASYNC(self, sendToSimulator, 0x6);
	} else if (self->previousQueue == 1) {
		self->previousQueue = 0;
		ASYNC(self, sendToSimulator, 0x9);
	}
}


void bitUSART(Controller *self, uint8_t data) {
	ASYNC(self, updateSouth, data); // show instruction from pc
	
	if ((data & 1) == 1) {					// Northbound car arrival sensor activated
		self->northQueue++;
		ASYNC(self, trafficLights, 0);
	} else if (((data >> 1) & 1) == 1) {	// Northbound bridge entry sensor activated
		if (self->northQueue > 0) {
			self->northQueue--;
		}
		ASYNC(self, trafficLights, 0);
	} else if (((data >> 2) & 1) == 1) {	// Southbound car arrival sensor activated
		self->southQueue++;
		ASYNC(self, trafficLights, 0);
	} else if (((data >> 3) & 1) == 1) {	// Southbound bridge entry sensor activated
		if (self->southQueue > 0) {
			self->southQueue--;
		}
		ASYNC(self, trafficLights, 0);
	}
}

void sendToSimulator(Controller *self, uint8_t data) {
	ASYNC(self, updateNorth, data); // show instruction from avr
	UDR0 = data;	// send instruction to pc
}