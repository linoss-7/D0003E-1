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
	int pos;
} LCD;

void writeChar(char ch, int pos);
void printAt(int num, int pos);
void init(void);
void updateLCD(LCD *self, int arg);
void change(PulseGenerator *self, int pulse);

#define initLCD(g1, g2, pos) {initObject(), g1, g2, pos}

#endif /* LCD_H_ */