/*	Author: Ngoc Nguyen
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 7  Exercise 2
 *	Exercise Description: The MAX value is the highest value of the ADC when most light detected, 
 *                           and is represented by 10 LEDs.
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo link: https://youtu.be/_c1tUqQKWMQ
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned short x ;
void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}
void main() {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
    /* Insert your solution below */
	ADC_init();
    while (1) {
	x = ADC;
	unsigned char tmpB = (char)x;
	unsigned char tmpD = (char)x;
	tmpD = (char)(x >> 8);

	PORTB= tmpB;
	PORTD = tmpD;
    }
    return 0;
}
