/*
 * LCD.h
 *
 * Created: 2017-02-21 14:31:29
 *  Author: Robin Andersson
 */ 


#ifndef LCD_H_
#define LCD_H_

void writeChar(char ch, int pos);
void printAt(long num, int pos);
void change(int pos);

#define initLCD() {initObject()}

#endif /* LCD_H_ */