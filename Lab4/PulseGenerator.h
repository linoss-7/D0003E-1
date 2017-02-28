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
	int portBit;
	int frequency;
	int saved;
	int oldFrequency;
	int firstPress;
} PulseGenerator;

void increasePulse(PulseGenerator *self, int arg);
void decreasePulse(PulseGenerator *self, int arg);
void saveState(PulseGenerator *self, int arg);
void goToPort(PulseGenerator *self, int arg);
void porting(PulseGenerator *self, int arg);

#define initPulseGenerator(number, freq, save, old, fst) {initObject(), number, freq, save, old, fst}

#endif /* PULSEGENERATOR_H_ */