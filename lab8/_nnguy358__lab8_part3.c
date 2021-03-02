/*	Author: Ngoc Nguyen
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab 8  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo link: https://youtu.be/zhdk5aN8L8w
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
unsigned char duration = 31;
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
void My_sunshine(){

	if(duration > 30) set_PWM(Freq[0]);
	else if (duration > 29) set_PWM(Freq[0]);
	else if (duration > 28) set_PWM(Freq[1]);
	else if (duration > 26) set_PWM(Freq[2]);
	else if (duration > 24) set_PWM(Freq[2]);

	else if (duration > 23) set_PWM(0);
	else if (duration > 22) set_PWM(Freq[2]);
	else if (duration > 21) set_PWM(Freq[1]);
	else if (duration > 20) set_PWM(Freq[2]);

	else if (duration > 18) set_PWM(Freq[0]);
	else if (duration > 16) set_PWM(Freq[0]);

	else if (duration > 15) set_PWM(0);
	else if (duration > 14) set_PWM(Freq[0]);
	else if (duration > 13) set_PWM(Freq[1]);
	else if (duration > 12) set_PWM(Freq[2]);

	else if (duration > 10) set_PWM(Freq[3]);
	else if (duration > 8) set_PWM(Freq[5]);

	else if (duration > 7) set_PWM(0);
	else if (duration > 6) set_PWM(Freq[5]);
	else if (duration > 5) set_PWM(Freq[4]);
	else if (duration > 4) set_PWM(Freq[3]);

	else if (duration > 0) set_PWM(Freq[2]);


	duration --;
}
enum States{ init, ON, OFF, release}state;
void Tick(){
	unsigned char button= ~PINA & 0x01;

	switch(state){
		case init: 
		state = OFF;
		break;

		case OFF:
		if(button ){ state = ON; i=0;}
		else state = OFF;
		break;

		case ON:
		if(i <= 31 ) state = ON; 
		else if (i >31){ state = release; }
		break;

		case release:
		if(!button  ) state = OFF; 
		else if (button){ state = release; }
		break;
				
		default:
		state = init;
		break;
	}
	
	switch(state){
		case init:
		break;
		
		case OFF:
		duration = 31;
		break;

		case ON:
		My_sunshine();
		i++;
		break;
		case release:
		set_PWM(0);
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
	TimerSet(40);
	TimerOn();
	state = init;
    while (1) {	
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;

    }
    return 1;
}


