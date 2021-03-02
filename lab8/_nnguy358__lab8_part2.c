/*	Author: Ngoc Nguyen
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 8  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo link: https://youtu.be/R3g9oHAUpsk
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
volatile unsigned char TimerFlag=0;
unsigned long _avr_timerM=1;
unsigned long _avr_timer_cntcurr=0;
unsigned char i = 0x00;
unsigned short Freq[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timerM;
	SREG |= 0x80;
}
void TimerOFF(){
	TCCR1B = 0x00;
}
void TimerIRS() {
   	TimerFlag = 1;
}
ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr==0){
		TimerIRS();
		_avr_timer_cntcurr = _avr_timerM;
	}
}
void TimerSet (unsigned long M){
	_avr_timerM = M;
	_avr_timer_cntcurr = _avr_timerM;
} 

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
enum States{ init, ON, OFF , cntUP, cntDOWN}state;
void Tick(){
	unsigned char button= ~PINA & 0x0F;

	switch(state){
		case init: 
		state = OFF;
		break;

		case OFF:
		if(button == 0x01) state = ON; 
		break;

		case ON:
		if(button == 0x02) {state = cntUP; }
		else if (button == 0x04){ state = cntDOWN; }
		else if (button == 0x01){ state = OFF;}
		break;
		
		case cntUP:
		state = ON;
		break;

		case cntDOWN:
		state = ON;
		break;
		
		default:
		state = init;
		break;
	}
	
	switch(state){
		case init:
		break;
		
		case OFF:
		set_PWM(0);
		break;

		case ON:
		set_PWM(Freq[i]);
		break;

		case cntUP:
		if(i<7) i++;
		break;

		case cntDOWN:
		if(i>0) i--;
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
	TimerSet(20);
	TimerOn();
    while (1) {	
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;

    }
    return 1;
}


