/*
 * General_Functions.h
 *
 * Created: 1/26/2020 3:48:41 PM
 *  Author: chand
 */ 


#ifndef GENERAL_FUNCTIONS_H_
#define GENERAL_FUNCTIONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define External_Interrupt4  E, 4

void Initialize_External_Interrupt_4()
{
	DDRE &= ~(1<<4);
	PORTE &= ~(1<<4);
// 	INPUT(External_Interrupt4);
// 	CLEAR(External_Interrupt4);

	EICRA |= (1 << ISC41);   // | (1 << ISC00);          //rising Edge
	EIMSK |= (1 << INT4);
	EIFR  |= (1 << INTF4);
}


void InitializePIDTimer()
{
	TCCR3B |= (1<<CS31) | (1<<CS30) | (1 << WGM32);	 //prescaler of 1024
	TCCR3A |= (1 << COM3A0);
	TCNT3   = 0;		        
	OCR3A   = 100;            //50ms
	TIMSK3 |= (1<<OCIE3A);
	TIFR3 |= (1<<OCF3A);	
}

#endif /* GENERAL_FUNCTIONS_H_ */