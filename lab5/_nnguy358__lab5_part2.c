/*	Author: Ngoc Nguyen
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #5  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_

#include "simAVRHeader.h"
#endif

enum States {Start, Init, Wait, Incre, Decre, reset} state;
unsigned char button; 
unsigned char preButtonState = 0x00; 
unsigned char tmpC;  

void Tick() {
	switch(state) { // Transitions
	case Start: 
	state = Init;
	break;
	case Init:
	state = Wait;
	break;
	case Wait:
	if( (button & 0x01) != (preButtonState & 0x01) ) {
	preButtonState = button;
	state = Incre;
	} else if((button & 0x02) != (preButtonState &0x02) ){
	preButtonState = button;
	state = Decre;
	} else if((button & 0x03) != (preButtonState & 0x03)){
	preButtonState = button;
	state = reset;
	}break;
	case Incre: 
	state = Wait;	
	break;
	case Decre:
	state = Wait;
	break;
	case reset:
	state = Wait;		
	break;
	default: state = Start; break;
}; // Transitions
	switch(state) { // State actions
	case Start: break;
	case Init: 
	tmpC=0x07; break;
	case Wait: break;
	case Incre: 
	if(tmpC<0x09)
	tmpC++;break;
	case Decre: 
	if(tmpC>0x00)
	tmpC--;break;
	case reset: tmpC=0x00;break;
	default: break;
 }; // State actions
}
 int main(void) {
	
    /* Insert DDR and PORT initializations */
        DDRA = 0x00;  PORTA = 0xFF;
        DDRC = 0xFF;  PORTC =0x00;

    /* Insert your solution below */
	button = PINA & 0x03;
	state = Start;
    while (1) {
	Tick(); 
	PORTC = tmpC;
    }
	
return 1;
}

