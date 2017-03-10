/*
 * USARTInterrupt.c
 *
 * Created: 2017-03-05 13:28:24
 *  Author: Robin Andersson
 */ 

#include "USARTinterrupt.h"

void receivedUSART(USART *self){
	uint8_t Data = UDR0;
	ASYNC(self->controller, bitUSART, Data);
}