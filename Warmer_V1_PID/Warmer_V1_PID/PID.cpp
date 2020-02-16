/*
 * PID.cpp
 *
 * Created: 1/29/2016 1:04:42 PM
 *  Author: YinYang
 */  
#include "PID.h"
#include "header.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void constrain(float &val,float minVal,float maxVal)
{
	if(val < minVal)
		val = minVal;
	if(val > maxVal)
		val = maxVal;
}

void PID::Initialize()
{
	kp         =  0;
	ki         =  0;
	kd         =  0;
	error      =  0;
	errSum     =  0;
	lastinput  =  0;
	lasterror  =  0;
	offset     =  0;
	setPoint   =  0;
	PID_Flag   =  false;
}
void PID::Set_PID(float KP,float KI,float KD)
{
	kp = KP;
	ki = KI;
	kd = KD;
}
float PID::Compute_PID(float input)
{
		error   = setPoint - input;
	
		
		
		Iterm += (ki*error);
		
		if (Iterm > 5000)
		{
			Iterm = 5000;
		}
		else if(Iterm < -5000)
		{
			Iterm = -5000;
		}
		float dErr = (error - lasterror);
		output = kp * error + Iterm - kd * dErr;
		constrain(output,minOut,maxOut);
		//Remember some variables for next time
		//lastinput = input;
		lasterror = error;
	return output;
}
