/*
 * Controller.h
 *
 * Created: 2017-03-05 13:32:58
 *  Author: Robin Andersson
 */ 


#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "TinyTimber.h"
#include "avr/io.h"

typedef struct{
	Object super;
	int northQueue;
	int southQueue;
	int previousQueue;
} Controller;

void trafficLights(Controller *self, int arg);
void bitUSART(Controller *self, uint8_t data);
void sendToSimulator(Controller *self, uint8_t data);

#define initController(north, south, prev) {north, south, prev}

#endif /* CONTROLLER_H_ */