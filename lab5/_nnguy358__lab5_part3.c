/*	Author: Ngoc
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 5  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * Demo Link: 
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
/*unsigned char GetBit(unsigned char x, unsigned char k) {
   return ((x & (0x01 << k)) != 0);
}*/
enum States {Start,Init , NextLed} state;
unsigned char A0; 
unsigned char preA0State;
unsigned char tmpB;  
int cnt;
void Tick() {
	switch(state) { // Transitions
	case Start: state = S0; break;
	case Init: 
	if(A0 != preA0State) {	
	state = S;
	} 
	case NextLed:
	if(A0 != preA0State)  {
	state = S0;
	}
	default: state = Start; break;
}; // Transitions
	switch(state) { // State actions
	case Start: break;

	case Init: 
	tmpB=0x00;
	break;
	case NextLed: 
	if(cnt<6){
	tmpB++;
	cnt++;
	}else if(cnt >=6){
	
	}
	break;
	default: break;
 }; // State actions
}
 int main(void) {
	
    /* Insert DDR and PORT initializations */
        DDRA = 0x00;  PORTA = 0xFF;
        DDRB = 0xFF;  PORTB =0x00;

    /* Insert your solution below */
	A0 = PINA & 0x01;
	state = Start;
    while (1) {
	Tick(); 
	PORTB = tmpB;
    }
	
return 1;
}
