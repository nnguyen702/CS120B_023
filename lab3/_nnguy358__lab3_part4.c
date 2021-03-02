/*	Author: Ngoc
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 3  Exercise 4
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
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char tmpA = 0x00; 

    /* Insert your solution below */
    while (1) {
	tmpA = PINA;
	
	PORTB = (tmpA >> 4) & 0x0F;
	PORTC = (tmpA << 4) & 0xF0;
    }
    return 1;
}
