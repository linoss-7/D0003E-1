#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "tinythreads.h"

#define NULL            0
#define DISABLE()       cli()
#define ENABLE()        sei()
#define STACKSIZE       80
#define NTHREADS        4
#define SETSTACK(buf,a) *((unsigned int *)(buf)+8) = (unsigned int)(a) + STACKSIZE - 4; \
*((unsigned int *)(buf)+9) = (unsigned int)(a) + STACKSIZE - 4

struct thread_block {
	void (*function)(int);   // code to run
	int arg;                 // argument to the above
	thread next;             // for use in linked lists
	jmp_buf context;         // machine state
	char stack[STACKSIZE];   // execution stack space
};

struct thread_block threads[NTHREADS];
struct thread_block initp;

thread freeQ   = threads;
thread readyQ  = NULL;
thread current = &initp;

int initialized = 0;
int timeCounter = 0;

int readCounter(void) {
	return timeCounter;
}

void resetCounter(void) {
	timeCounter = 0;
}

static void initialize(void) {
	int i;
	for (i=0; i<NTHREADS-1; i++)
	threads[i].next = &threads[i+1];
	threads[NTHREADS-1].next = NULL;
	
	initialized = 1;
	
	// our code
	
	// disables clock prescaler
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// enables the logical interrupt source PCINT15
	EIMSK = (1 << PCIE1);
	PCMSK1 = (1 << PCINT15);
	
	PORTB = 0x80; // activates a pull-up resistor
	
	
	TCCR1A = (1 << COM1A1) | (1 << COM1A0); // sets OC1A high on compare match
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // set the timer to Clear on Timer Compare and set timer prescaler factor to 1024
	OCR1A = 8000000/20480; // set a suitable value to OCR1A
	TCNT1 = 0x0; // clears the TCNT1 register
	TIMSK1 = (1 << OCIE1A); // enables timer output compare A interrupts
	
	// LCD enabled, low power waveform, no frame interrupt, no blanking
	LCDCRA = (1 << LCDEN) | (1 << LCDAB);
	// external asynchronous clock source, 1/3 bias, 1/4 duty cycle, 25 segments enabled
	LCDCRB = (1 << LCDCS) | (1 << LCDMUX0) | (1 << LCDMUX1) | (1 << LCDPM0) | (1 << LCDPM1) | (1 << LCDPM2);
	// prescaler setting N=16, clock divider setting D=8
	LCDFRR = (1 << LCDCD0) | (1 << LCDCD1) | (1 << LCDCD2);
	// drive time 300 microseconds, contrast control voltage 3.35 V
	LCDCCR = (1 << LCDCC0) | (1 << LCDCC1) | (1 << LCDCC2) | (1 << LCDCC3);
}

static void enqueue(thread p, thread *queue) {
	p->next = NULL;
	if (*queue == NULL) {
		*queue = p;
	} else {
		thread q = *queue;
		while (q->next)
		q = q->next;
		q->next = p;
	}
}

static thread dequeue(thread *queue) {
	thread p = *queue;
	if (*queue) {
		*queue = (*queue)->next;
		} else {
		// Empty queue, kernel panic!!!
		while (1) ;  // not much else to do...
	}
	return p;
}

static void dispatch(thread next) {
	if (setjmp(current->context) == 0) {
		current = next;
		longjmp(next->context,1);
	}
}

void spawn(void (* function)(int), int arg) {
	thread newp;

	DISABLE();
	if (!initialized) initialize();

	newp = dequeue(&freeQ);
	newp->function = function;
	newp->arg = arg;
	newp->next = NULL;
	if (setjmp(newp->context) == 1) {
		ENABLE();
		current->function(current->arg);
		DISABLE();
		enqueue(current, &freeQ);
		dispatch(dequeue(&readyQ));
	}
	SETSTACK(&newp->context, &newp->stack);

	enqueue(newp, &readyQ);
	ENABLE();
}

void yield(void) {
	DISABLE();
	enqueue(current, &readyQ);
	dispatch(dequeue(&readyQ));
	ENABLE();
}

void lock(mutex *m) {
	DISABLE();
	if (m->locked == 0) {
		m->locked = 1;
	} else {
		enqueue(current, &(m->waitQ));
		dispatch(dequeue(&readyQ));
	}
	ENABLE();
}

void unlock(mutex *m) {
	DISABLE();
	if (m->waitQ != NULL) {
		enqueue(current, &readyQ);
		dispatch(dequeue(&(m->waitQ)));
	} else {
		m->locked = 0;
	}
	ENABLE();
}

// Defines interrupt handler for PCINT1_vect
ISR(PCINT1_vect) {
	if (PINB >> 7 == 0) { // maskes sure that the button is actually depressed
		yield();
	}
}

// Defines timer interrupt for TIMER1_COMPA_vect
ISR(TIMER1_COMPA_vect) {
	timeCounter += 1;
	yield();
}
