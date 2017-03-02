/*
 * Port.c
 *
 * Created: 2017-03-01 15:26:33
 *  Author: Robin Andersson
 */ 

#include "PulseGenerator.h"
#include "TinyTimber.h"
#include "Port.h"
#include <avr/io.h>

void porting(PORT *self, int portBit) {
	if (portBit == 4) {
		PORTE ^= 0x10;
	} else if (portBit == 6) {
		PORTE ^= 0x40;
	}
}
