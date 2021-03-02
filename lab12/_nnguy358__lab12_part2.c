/*	Author: Ngoc Nguyen
 *  Partner(s) Name: 
 *	Lab Section:023
 *	Assignment: Lab 12  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo link: https://youtu.be/1nPgWWt1Ays
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif
//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------
typedef struct _task{
	// Tasks should have members that include: state, period,
	//a measurement of elapsed time, and a function pointer.
	signed 	 char state; 		//Task's current state
	unsigned long period; 		//Task period
	unsigned long elapsedTime; 	//Time elapsed since last task tick
	int (*TickFct)(int); 		//Task tick function
} task;

enum Demo_States {init, wait, right, left};
int Demo_Tick(int state) {

	// Local Variables
	static unsigned char pattern = 0x80;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0x00;  	// Row(s) displaying pattern. 
						// 0: display pattern on row
						// 1: do NOT display pattern on row
	 unsigned char button = ~PINA;
	// Transitions
	switch (state) {
		case init: state = wait;break;
		case wait:
			if(button == 0x01){state = right;}
			else if(button== 0x02){ state = left;}
			else {state = wait;}
		break;
		case right: state = wait; break;
		case left: state = wait; break;
		default: state = init; break;
	}	
	// Actions
	switch (state) {
		case wait:  break;
		case right: 
			if(pattern > 0x01){pattern >>= 1;}
			else {  pattern = 0x01;}
		break;
		case left: 
			if(pattern < 0x80){pattern <<= 1;}
			else { pattern = 0x80;}
		break;			
		default:break;
	}
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern	
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA=0x00; PORTA=0xFF;
	DDRC=0xFF; PORTC=0x00;
	DDRD=0xFF; PORTD=0x00;
    /* Insert your solution below */
	static task task1;
	task *tasks[] = { &task1};
	const unsigned char numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;
	
	task1.state = start;	
	task1.period=100;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Demo_Tick;

	TimerSet(100);
	TimerOn();
	
	unsigned short i;
    while (1) {
	for (i=0; i<numTasks; i++){
		if(tasks[i]->elapsedTime == tasks[i]->period){
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;		
		}
		tasks[i]->elapsedTime = 100;
	
	}
	while (!TimerFlag);
	TimerFlag=0;
	
    }
    return 0;
}


