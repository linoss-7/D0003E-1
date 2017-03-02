/*
 * Port.h
 *
 * Created: 2017-03-01 15:26:44
 *  Author: Robin Andersson
 */ 


#ifndef PORT_H_
#define PORT_H_

#include "PulseGenerator.h"
#include "TinyTimber.h"

typedef struct {
	Object super;
	int currentPulse;
} PORT;

void porting(PORT *self, int portBit);

#define initPORT(pulse) {initObject(), pulse}

#endif /* PORT_H_ */