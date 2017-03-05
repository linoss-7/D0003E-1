/*
 * Controller.c
 *
 * Created: 2017-03-05 10:39:29
 * Author : Robin Andersson
 */ 

#include <avr/io.h>
#include "TinyTimber.h"
#include "Controller.h"
#include "USARTInterrupt.h"
#include "LCD.h"

LCD lcd = initLCD();
Controller controller = initController(&lcd, 0, 0, 0);
USART usart = initUSARTInterrupt(&controller);

int main(void) {
   init();
   
   INSTALL(&usart, receivedUSART, IRQ_USART0_RX);
   return TINYTIMBER(&lcd, updateNorth, 0);
}

