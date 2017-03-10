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

Controller controller = initController(0, 0, 0);
USART usart = initUSARTInterrupt(&controller);

int main(void) {
   init();
   
   INSTALL(&usart, receivedUSART, IRQ_USART0_RX);
   return TINYTIMBER(&controller, updateNorth, 0);
}
