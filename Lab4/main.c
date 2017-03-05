/*
 * Lab4.c
 *
 * Created: 2017-02-21 11:33:14
 * Author : Robin Andersson
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "PulseGenerator.h"
#include "GUI.h"
#include "TinyTimber.h"
#include "LCD.h"
#include "Port.h"

PORT p = initPORT(1);
PulseGenerator g1 = initPulseGenerator(4, 0, 0, 0, &p);	
PulseGenerator g2 = initPulseGenerator(6, 0, 0, 0, &p);
LCD lcd = initLCD(&g1, &g2, &g1);
AsyncHandler ah = initAsyncHandler(&lcd);
GUI gui = initGUI(&lcd, &ah);


int main(void) {
  init();
  
  INSTALL(&gui, changePortE, IRQ_PCINT0);
  INSTALL(&gui, changePortB, IRQ_PCINT1);
  
  return TINYTIMBER(&lcd, updateLCD, 0);
}

