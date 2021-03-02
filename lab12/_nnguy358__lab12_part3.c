/*	Author:Ngoc Nguyen
 *  Partner(s) Name: 
 *	Lab Section:023
 *	Assignment: Lab 12  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo link: https://youtu.be/NKfSPgsJfw4
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

enum Demo_States {row1,row2, row3,row4, row5};
int Demo_Tick(int state) {

	// Local Variables
	static unsigned char pattern = 0x00;
	static unsigned char patternAR[5] = {0x00,0x3C,0x24,0x3C,0x00};//LED pattern-0:LED off;1: 
								//LED on
	static unsigned char row = 0xFF;
	static unsigned char rowAR[5]= {0xFF,0xFD,0xFB,0xF7,0xFF};// Row(s) displaying pattern. 
							// 0: display pattern on row
							// 1: do NOT display pattern on row
	unsigned short i;
	// Transitions
	switch (state) {
		case row1: state = row2; break;
		case row2: state = row3; break;
		case row3: state = row4; break;
		case row4: state = row5; break;
		case row5: state = row1; break;
		default: state = row1; break;
	}	
	// Actions
	switch (state) {
		case row1: pattern = patternAR[1];row = rowAR[1];break; 		
		case row2: pattern = patternAR[2];row = rowAR[2];break;			
		case row3: pattern = patternAR[3];row = rowAR[3];break;
		case row4: pattern = patternAR[4];row = rowAR[4];break;			
		case row5: pattern = patternAR[5];row = rowAR[5];break;			
		default:break;
	}
	PORTC = pattern;	// Pattern to display
	PORTD = row;		// Row(s) displaying pattern	
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRC=0xFF; PORTC=0x00;
	DDRD=0xFF; PORTD=0x00;
    /* Insert your solution below */
	static task task1;
	task *tasks[] = { &task1};
	const unsigned char numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;
	
	task1.state = start;	
	task1.period=1;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Demo_Tick;

	TimerSet(1);
	TimerOn();
	
	unsigned short i;
    while (1) {
	for (i=0; i<numTasks; i++){
		if(tasks[i]->elapsedTime == tasks[i]->period){
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;		
		}
		tasks[i]->elapsedTime = 1;
	
	}
	while (!TimerFlag);
	TimerFlag=0;
	
    }
    return 0;
}


