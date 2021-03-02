/*	Author: Ngoc Nguyen
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 8  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo link: https://youtu.be/ytGpVHrBGDw
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum States{ init, wait , C_note, E_note, D_note}state;
void set_PWM(double frequency){
	static double current_frequency;
	if(frequency != current_frequency){
	if(!frequency){TCCR3B &= 0x08;}	
	else{TCCR3B |= 0x03;}
	
	if(frequency < 0.954){OCR3A = 0xFFFF;}	

	else if(frequency > 31250){OCR3A &= 0x000;}
	
	else{OCR3A = (short)( 8000000 / (128 * frequency)) - 1; }
	
	TCNT3 = 0;
	current_frequency = frequency;
	}
	}
void PWM_on(){
	TCCR3A = (1<< COM3A0);
	TCCR3B = (1<< WGM32)| (1<< CS31) | (1<<CS30);
	set_PWM(0);
}
void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

void Tick(){
	unsigned char button = ~PINA & 0x0F;

	switch(state){
		case init: 
		state = wait;
		break;

		case wait:
		if(button == 0x01){ state = C_note; }
		else if (button == 0x02){ state = D_note; }
		else if (button == 0x04){ state = E_note; }
		else { state = wait;}
		break;

		case C_note:
		if(button == 0x01){ state = C_note; }
		else{state = wait;}
		break;
		
		case D_note:
		if(button == 0x02){ state = D_note; }
		else{state = wait;}
		break;

		case E_note:
		if(button == 0x04){ state = E_note; }
		else{state = wait;}
		break;
		
		default:
		state = init;
		break;
	}
	
	switch(state){
		case init:
		break;
		
		case wait:
		set_PWM(0);
		PORTB = 0x40;
		break;

		case C_note:
		set_PWM(261.63);
		PORTB = 0x41;
		break;

		case D_note:
		set_PWM(293.66);
		PORTB = 0x42;
		break;

		case E_note:
		set_PWM(329.63);
		PORTB = 0x44;
		break;
	
		default:
		break;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	PWM_on();
    while (1) {	

	Tick();

    }
    return 0;
}


