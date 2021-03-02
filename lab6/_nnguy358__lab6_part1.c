/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo link: https://youtu.be/dXVd_LA2000
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include <avr/interrupt.h>

#include "simAVRHeader.h"
#endif
volatile unsigned char TimerFlag=0; // ISR raises, main() lowers
unsigned long _avr_timerM=1;
unsigned long _avr_timer_cntcurr=0;
unsigned char tmpB = 0x00;
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
enum TL_States { TL_SMStart, TL_s1, TL_s2, TL_s3} TL_State;

void TickFct_BlinkTL() {

  switch(TL_State) { // Transitions
     case TL_SMStart: // Initial transition
        TL_State = TL_s1;
        break;
     case TL_s1:
        TL_State = TL_s2;
        break;
     case TL_s2:
        TL_State = TL_s3;
        break;
     case TL_s3:
        TL_State = TL_s1;
        break;
     default:
        TL_State = TL_SMStart;
   } // Transitions

  switch(TL_State) { // State actions
     case TL_s1:
        tmpB = 0x01;
        break;
     case TL_s2:
        tmpB = 0x02;
        break;
     case TL_s3:
        tmpB = 0x04;
        break;
     default:
        break;
  } // State actions
}

void main() {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
  	TimerSet(100);
  	TimerOn();
	TL_State = TL_SMStart;
while (1) {         
      TickFct_BlinkTL();      // Execute one synchSM tick
      PORTB = tmpB;
      while (!TimerFlag){}  // Wait for BL's period
      TimerFlag = 0;        // Lower flag

  }

}


