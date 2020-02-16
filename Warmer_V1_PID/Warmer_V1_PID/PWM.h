/*
 * PWM.h
 *
 * Created: 12/27/2019 2:36:05 PM
 *  Author: chand
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include "header.h"

#define ICR_TOP      4999     //4999
#define MAX_VALUE    4999
#define MIN_VALUE    -4999   //249

#define PWM_PIN      B, 5
#define PWM_TCCRA    TCCR1A
#define PWM_TCCRB    TCCR1B
#define PWM_ICR      ICR1
#define PWM_OCR      OCR1A

#define PWM_COM0     COM1A0
#define PWM_COM1     COM1A1

#define PWM_WGM0     WGM10
#define PWM_WGM1     WGM11
#define PWM_WGM2     WGM12
#define PWM_WGM3     WGM13
#define PWM_CS0      CS10
#define PWM_CS1      CS11
#define PWM_CS2      CS12


class PWM
{
	public:
		void InitPWM();
		void SetOcrValue(int x);	
};


#endif /* PWM_H_ */