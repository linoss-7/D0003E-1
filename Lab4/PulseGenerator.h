/*
 * PulseGenerator.h
 *
 * Created: 2017-02-21 11:49:33
 *  Author: Robin Andersson
 */ 

#ifndef PULSEGENERATOR_H_
#define PULSEGENERATOR_H_

#include "TinyTimber.h"
#include "Port.h"

typedef struct {
	Object super;
	int portBit;
	int frequency;
	int saved;
	int oldFrequency;
	PORT *port;
} PulseGenerator;

void increasePulse(PulseGenerator *self, int arg);
void decreasePulse(PulseGenerator *self, int arg);
void saveState(PulseGenerator *self, int arg);
void goToPort(PulseGenerator *self, int arg);

#define initPulseGenerator(number, freq, save, old, port) {initObject(), number, freq, save, old, port}

#endif /* PULSEGENERATOR_H_ */