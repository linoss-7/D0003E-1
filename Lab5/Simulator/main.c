/*
 * Simulator.c
 *
 * Created: 2017-03-05 10:40:59
 * Author : Robin Andersson
 */ 

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#define SERIAL_PORT "/dev/ttyS0"

int openserialport();
void serialWrite(uint8_t);
int serialread();
void input();
void *GUI(void *a);
void *trafficLights(void *a);
void *Bridge(void *a);

pthread_t controll;
pthread_t gui;
pthread_t bridge;

int fd;
int northCars = 0;
int southCars = 0;
int redSouth = 1;
int redNorth = 1;
int greenSouth = 0;
int greenNorth = 0;
int i = 0;
int x = 10;
int j;
uint8_t serialIn;


int main(void) {
	printf("\033[2J");
	openserialport(); // Initializes the serial port with 9600 Baud, 8 start bits, 2 stop bits.
	pthread_create(&controll, NULL, trafficLights, 0); // thread, attributes?, method and argument
	//serialread();
	pthread_create(&gui, NULL, GUI, 0); // thread, attributes?, method and argument
	pthread_create(&bridge, NULL, Bridge, 0);
	input();
}

void *Bridge(void *a) {
	while (1) {
		while (greenNorth == 1 && redSouth == 1) {
			northCars--;
			serialWrite(0x2)
			usleep(10000000);
		} 
	}
}

void *trafficLights(void *a) {
	while(1) {
		int data = read(fd, &serialIn, sizeof(serialIn));
		if (data > 0) {
			if (serialIn == 0x6) {
				greenNorth = 0;
				redNorth = 1;
				greenSouth = 1;
				redSouth = 0;
			} else if (serialIn == 0x9) {
				greenNorth = 1;
				redNorth = 0;
				greenSouth = 0;
				redSouth = 1;
			} else if (serialIn == 0xA) {
				greenNorth = 0;
				redNorth = 1;
				greenSouth = 0;
				redSouth = 1;
			}
		}
		usleep(50000);
	}
}

