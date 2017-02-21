/*
 * PulseGenerator.h
 *
 * Created: 2017-02-21 11:49:33
 *  Author: Robin Andersson
 */ 

#ifndef PULSEGENERATOR_H_
#define PULSEGENERATOR_H_

#include "TinyTimber.h"

typedef struct {
	Object super;
	int bitNumber;
	int frequency;
	int saved;
	int oldFrequency;
} PulseGenerator;

void increasePulse(PulseGenerator *self, int arg);
void decreasePulse(PulseGenerator *self, int arg);
void saveState(PulseGenerator *self, int arg);
void goToPort(PulseGenerator *self);

#define initPulseGenerator(number) {initObject(), number, 0, 0, 0}

#endif /* PULSEGENERATOR_H_ */