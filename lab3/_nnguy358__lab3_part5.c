/*	Author: Ngoc
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 3  Exercise 5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
unsigned char GetBit(unsigned char x, unsigned char k) {
   return ((x & (0x01 << k)) != 0);
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0x00; PORTB = 0x01;
	DDRB = 0xF7; PORTB = 0x00;
	
	unsigned char tmpD = 0x00; 
	unsigned char tmpB = 0x00; 
	unsigned short weight;
	unsigned char tmpB_out = 0x00; 

    /* Insert your solution below */
    while (1) {
	tmpD = PIND;
	tmpB = PIND;
	weight = (tmpD<<1) + tmpB;
	if(weight>= 0x46){
	tmpB_out = 0x02;	
	}
	else if(weight>=0x05 && weight<=0x46){
	tmpB_out = 0x04;
	}
	else if(weight<=0x05 ){
	tmpB_out = 0x00;
	}
	
	
	PORTB = tmpB_out;

    }
    return 1;
}
