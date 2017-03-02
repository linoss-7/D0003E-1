/*
 * AsyncHandler.h
 *
 * Created: 2017-02-27 14:16:31
 *  Author: Robin Andersson
 */ 


#ifndef ASYNCHANDLER_H_
#define ASYNCHANDLER_H_

#include "PulseGenerator.h"
#include "LCD.h"

typedef struct {
	Object super;
	PulseGenerator *currentPulse;
	PulseGenerator *g1;
	PulseGenerator *g2;
	LCD *lcd;
	int firstPress;
} AsyncHandler;

void hold(AsyncHandler *self, int arg);

#define initAsyncHandler(pulse, g1, g2, lcd) {initObject(), pulse, g1, g2, lcd}

#endif /* ASYNCHANDLER_H_ */