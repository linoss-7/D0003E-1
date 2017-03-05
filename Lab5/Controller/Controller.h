/*
 * Controller.h
 *
 * Created: 2017-03-05 13:32:58
 *  Author: Robin Andersson
 */ 


#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "TinyTimber.h"
#include "LCD.h"

typedef struct{
	Object super;
	LCD *lcd;
	int northQueue;
	int southQueue;
	int previousQueue;
} Controller;

void trafficLights(Controller *self, int arg);
void bitUSART(Controller *self, uint8_t data);
void sendToSimulator(Controller *self, uint8_t data);

#define initController(lcd, north, south, prev) {lcd, north, south, prev}

#endif /* CONTROLLER_H_ */