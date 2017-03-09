/*
 * LCD.h
 *
 * Created: 2017-03-05 10:48:18
 *  Author: Robin Andersson
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "TinyTimber.h"
#include "Controller.h"
#include <avr/io.h>

void writeChar(char ch, int pos);
void printAt(long num, int pos);
void init();
void updateNorth(Controller *self, int data);
void updateSouth(Controller *self, int data);
void updateStatus(Controller *self, int whichInterrupt);


#endif /* LCD_H_ */