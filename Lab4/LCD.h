/*
 * LCD.h
 *
 * Created: 2017-02-21 14:31:29
 *  Author: Robin Andersson
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "PulseGenerator.h"
#include "TinyTimber.h"

typedef struct {
	Object super;
	PulseGenerator *g1;
	PulseGenerator *g2;
} LCD;

void writeChar(LCD *self, char ch, int pos);
void printAt(LCD *self, int num, int pos);
void init(LCD *self, int arg);
void updateLCD(LCD *self, int arg);
void change(LCD *self, int pulse);

#define initLCD(pg1, pg2) {initObject(), pg1, pg2}

#endif /* LCD_H_ */