/*
 * PWM.cpp
 *
 * Created: 12/27/2019 3:40:41 PM
 *  Author: chand
 */ 

#include "PWM.h"

void PWM::InitPWM()
{
	OUTPUT(PWM_PIN);
	
	PWM_TCCRA |= (1 << PWM_COM1) | (1 << PWM_WGM1);       //Fast PWM    Set OC1A on compare match , clear OC1A at Bottom (inverting mode)  | (1 << PWM_COM0)
	PWM_TCCRB |= (1 << PWM_WGM2) | (1 << PWM_WGM3) | (1 << PWM_CS1);
	PWM_ICR    = ICR_TOP;
}

void PWM::SetOcrValue(int ocr)
{
	if(ocr >= MAX_VALUE)
	{
		ocr = MAX_VALUE;
	}
// 	if(ocr <= MIN_VALUE);
// 	{
// 		ocr = MIN_VALUE;
// 	}
	PWM_OCR = ocr;
}