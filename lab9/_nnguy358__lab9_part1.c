/*	Author: Ngoc Nguyen
 *  Partner(s) Name:
 *	Lab Section:023
 *	Assignment: Lab 9  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo link: https://youtu.be/Cx-YlJ43SSE
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include <avr/interrupt.h>
#include "simAVRHeader.h"
#endif


volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1ms
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B 	= 0x0B;	// bit3 = 1: CTC mode (clear timer on compare)
					// bit2bit1bit0=011: prescaler /64
					// 00001011: 0x0B
					// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
					// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A 	= 125;	// Timer interrupt will be generated when TCNT1==OCR1A
					// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
					// So when TCNT1 register equals 125,
					// 1 ms has passed. Thus, we compare to 125.
					// AVR timer interrupt mask register

	TIMSK1 	= 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1 = 0;

	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	_avr_timer_cntcurr = _avr_timer_M;

	//Enable global interrupts
	SREG |= 0x80;	// 0x80: 1000000
}

void TimerOff() {
	TCCR1B 	= 0x00; // bit3bit2bit1bit0=0000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect)
{
	// CPU automatically calls when TCNT0 == OCR0 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; 			// Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { 	// results in a more efficient compare
		TimerISR(); 				// Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
unsigned char blinkingLED;
unsigned char threeLEDs;

enum BL_State{BL_Start, BL_on, BL_off}BL_State;
void BlinkLED_SM(){
	switch (BL_State){
		case BL_Start: BL_State =BL_on; break;
		case BL_on: BL_State =BL_off; break;
		case BL_off: BL_State =BL_on; break;
		default : BL_State = BL_Start; break;
	}
	switch (BL_State){
		case BL_Start: blinkingLED = 0x00; break;
		case BL_on: blinkingLED = 0x08; break;
		case BL_off: blinkingLED = 0x00; break;
		default : break;
	}

}
enum TL_State{TL_Start, TL_B0, TL_B1, TL_B2}TL_State;
void ThreeLEDs_SM(){
	switch (TL_State){
		case TL_Start: TL_State =TL_B0; break;
		case TL_B0: TL_State =TL_B1; break;
		case TL_B1: TL_State =TL_B2; break;
		case TL_B2: TL_State =TL_B0; break;
		default : TL_State = TL_Start;break;
	}
	switch (TL_State){
		case TL_Start: threeLEDs = 0x00; break;
		case TL_B0: threeLEDs = 0x01; break;
		case TL_B1: threeLEDs = 0x02; break;
		case TL_B2: threeLEDs = 0x04; break;
		default : break;
	}

}
enum CB_State{CB_Start, CB_out}CB_State;
void CombinedLEDs_SM(){
	switch (CB_State){
		case CB_Start: CB_State =CB_out; break;

		case CB_out: CB_State =CB_out; break;
		default : CB_State = CB_Start;break;
	}
	switch (CB_State){
		case CB_Start:  break;
		case CB_out: PORTB = blinkingLED | threeLEDs; break;
		default : break;
	}

}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	BL_State = BL_Start;
	TL_State = TL_Start;
	CB_State = CB_Start;
	TimerSet(100);
	TimerOn();
    while (1) {
	BlinkLED_SM();
	ThreeLEDs_SM();
	CombinedLEDs_SM();
	while(!TimerFlag){};
	TimerFlag = 0;

    }
    return 1;
}
