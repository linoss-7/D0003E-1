/*
 * GUI.h
 *
 * Created: 2017-02-21 14:02:27
 *  Author: Robin Andersson
 */ 


#ifndef GUI_H_
#define GUI_H_

#include "TinyTimber.h"
#include "PulseGenerator.h"
#include "LCD.h"

typedef struct {
	Object super;
	PulseGenerator currentPulse;
	PulseGenerator g1;
	PulseGenerator g2;
} GUI;

void updateLCD(GUI *self, int arg);
void changePortB(GUI *self, int arg);
void changePortE(GUI *self, int arg);

#define initGUI(pulse, pg1, pg2) {initObject(), pulse, pg1, pg2}

#endif /* GUI_H_ */