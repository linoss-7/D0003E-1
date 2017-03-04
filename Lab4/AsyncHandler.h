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
	LCD *lcd;
	int held;
} AsyncHandler;

void hold(AsyncHandler *self, int arg);

#define initAsyncHandler(lcd) {initObject(), lcd}

#endif /* ASYNCHANDLER_H_ */