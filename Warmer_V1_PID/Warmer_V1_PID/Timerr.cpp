/*
 * Timerr.cpp
 *
 * Created: 6/6/2019 8:04:17 AM
 *  Author: swain
 */ 

#include "Timerr.h"


void Timerr::setTimerNum(int timer_num) {
	this->timer_no = timer_num;
}

void Timerr::setTimerOverflow() {
	
	SET_OVERFLOW(2);	
}

void Timerr::startTimer() {
	
	START_TIMERR(2,0,1);
	
}

void Timerr::stopTimer() {
	
	STOP_TIMER(2);
}

void Timerr::setCompareInterrupt() {
	SET_COMPARE(2, A);
}

void Timerr::resetTimer() {
	
	TCNT2 = 0;
	
}
void Timerr::startCustomTimer(int milli_sec) {
	
		int ocr = (15.625 * milli_sec);
		OCR2A = ocr;
		START_TIMERR(2,0,2); //for proteus use 5,0,1 and for real life use 5,0,2
}