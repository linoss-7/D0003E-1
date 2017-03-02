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
#include "AsyncHandler.h"

typedef struct {
	Object super;
	PulseGenerator *currentPulse;
	PulseGenerator *g1;
	PulseGenerator *g2;
	LCD *lcd;
	AsyncHandler *ah;
} GUI;

int changePortB(GUI *self, int arg);
int changePortE(GUI *self, int arg);

#define initGUI(pulse, pg1, pg2, lcd, ah) {initObject(), pulse, pg1, pg2, lcd, ah}

#endif /* GUI_H_ */