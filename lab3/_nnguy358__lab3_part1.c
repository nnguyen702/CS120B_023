/*	Author: Ngoc
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 3  Exercise 1
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
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;

	unsigned char tmpB = 0x00; 
        unsigned char tmpA = 0x00; 
	unsigned char cntA;
	unsigned char cntB;

	unsigned char i;
	
    /* Insert your solution below */
    while (1) {
	tmpA = PINA;
	tmpB = PINB;
	cntA = 0;
	cntB = 0;
      	for (i=0; i<8; i++) {
         if (GetBit(tmpA, i)) {
            cntA++;
         }
	}
 	for (i=0; i<8; i++) {
         if (GetBit(tmpB, i)) {
            cntB++;
         }
	}

	PORTC = cntA + cntB;
    }
    return 1;
}
