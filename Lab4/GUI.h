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
	LCD *lcd;
	AsyncHandler *ah;
} GUI;

void changePortB(GUI *self, int arg);
void changePortE(GUI *self, int arg);

#define initGUI(lcd, ah) {initObject(), lcd, ah}

#endif /* GUI_H_ */