/*
 * USARTInterrupt.h
 *
 * Created: 2017-03-05 13:28:54
 *  Author: Robin Andersson
 */ 


#ifndef USARTINTERRUPT_H_
#define USARTINTERRUPT_H_

#include "Controller.h"
#include "TinyTimber.h"
#include "avr/io.h"

typedef struct{
	Object super;
	Controller *controller;
} USART;

void receivedUSART(USART *self);

#define initUSARTInterrupt(cont) {initObject(), cont};

#endif /* USARTINTERRUPT_H_ */