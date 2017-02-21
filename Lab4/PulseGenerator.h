/*
 * PulseGenerator.h
 *
 * Created: 2017-02-21 11:49:33
 *  Author: Robin Andersson
 */ 

#include "TinyTimber.h"

#ifndef PULSEGENERATOR_H_
#define PULSEGENERATOR_H_

typedef struct {
	Object super;
	uint8_t bitNumber;
	uint8_t frequency;
	uint8_t saved;
	uint8_t oldFrequency;
} PulseGenerator;

void increasePulse(PulseGenerator *self, int arg);
void decreasePulse(PulseGenerator *self, int arg);
void saveState(PulseGenerator *self, int arg);
void goToPort(PulseGenerator *self);

#define initPulseGenerator(number) {initObject(), number, 0, 0, 0}

#endif /* PULSEGENERATOR_H_ */