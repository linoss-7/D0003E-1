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

void openSerialPort();
void serialWrite(uint8_t instruction);
void input(void);
void *GUI(void *a);
void *trafficLights(void *a);
void *Bridge(void *a);

pthread_t controll;
pthread_t gui;
pthread_t bridge;

int COM1;
int northCars = 0;
int southCars = 0;
int redSouth = 1;
int redNorth = 1;
int greenSouth = 0;
int greenNorth = 0;




int main(void) {
	printf("\037[2J");
	openserialport(); // Initializes the serial port with 9600 Baud, 8 start bits, 2 stop bits.
	pthread_create(&controll, NULL, trafficLights, 0); // thread, attributes, method and argument
	pthread_create(&gui, NULL, GUI, 0); // thread, attributes, method and argument
	pthread_create(&bridge, NULL, Bridge, 0);
	input();
}

void *GUI(void *a) {
	while (1) {
		printf("\037[2J");
		
		printf("North: Red Light Status %d, Green Light Status %d, Number of cars in queue %d \n", greenNorth, redNorth, northCars);
		printf("South: Red Light Status %d, Green Light Status %d, Number of cars in queue %d \n", greenSouth , redSouth, southCars);
	}
}

void openserialport(void) {
	COM1 = open("/dev/ttyS0", O_RDWR);
	if (COM1 < 0){
		printf("Cannot open the file: %s\n", strerror(errno));
	}
	struct termios tty;
	if (tcgetattr(COM1, &tty)){
		printf("Error2\n");
		exit(-1);
	}
	tcflush(COM1, TCIFLUSH);
	tty.c_cflag = B9600 | CS8 | CSTOPB | CREAD | CLOCAL | HUPCL | INPCK;
	tty.c_lflag &= ~(ECHO | ECHONL | ICANON);
	tty.c_cc[VTIME] = 10;
	tty.c_cc[VMIN] = 1;
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);

	if(tcsetattr(COM1, TCSANOW, &tty)){
		printf("Error3\n");
		exit(-1);
	}
}

void serialWrite(uint8_t instruction){
	// Writes to the serial port.
	//printf("Writing!");
	int text = write(COM1, &instruction, sizeof(instruction));
	if (text < 0) {
		printf("Error4\n");
	}

void input(void) {
	fd_set rfds;
	fcntl(fileno(stdin), F_SETFL, O_NONBLOCK); // open stdin without delay?
	
	struct termios tty;
	tcgetattr(STDIN_FILENO, &tty);
	tty.c_lflag &= ~(ICANON | ECHO);
	tty.c_cc[VMIN] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
	
	FD_SET(0, &rfds); // include keyboard
			
	while (1) {
		if(FD_ISSET(0, &rfds)) { // handle keypress
			char ch = getchar();
			
			if (ch == 'a') { // North car
				serialWrite(0x1);
				northCars++;
			} else if (ch == 'd') { // South car
				serialWrite(0x4);
				southCars++;
			}
		}
		
	}
}

void *Bridge(void *a) {
	while (1) {
		while (greenNorth == 1) {
			northCars--;
			serialWrite(0x2)
			usleep(1000000);
		} 
		
		if (redSouth == 0) {
			usleep(5000000);
		} 
		
		while (greenSouth == 1) {
			southCars--;
			serialWrite(0x8);
			usleep(1000000);
		}
		
		if (redNorth == 0) {
			usleep(5000000);
		}
		
		fflush(stdin);
	}
}

void *trafficLights(void *a) {
	uint8_t serialIn;
	while(1) {
		int data = read(COM1, &serialIn, sizeof(serialIn));
		if (data > 0) {
			if (data == 0x6) {
				greenNorth = 0;
				redNorth = 1;
				greenSouth = 1;
				redSouth = 0;
			} else if (data == 0x9) {
				greenNorth = 1;
				redNorth = 0;
				greenSouth = 0;
				redSouth = 1;
			} else if (data == 0xA) {
				greenNorth = 0;
				redNorth = 1;
				greenSouth = 0;
				redSouth = 1;
			}
		}
		// usleep(50000); not needed?
	}
}

