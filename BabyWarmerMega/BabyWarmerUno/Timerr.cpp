/*
 * Timerr.cpp
 *
 * Created: 6/6/2019 8:04:17 AM
 *  Author: swain
 */ 

#include "Timerr.h"
#include "Header.h"


void Timerr::setTimerNum(int timer_num) {
	this->timer_no = timer_num;
}

void Timerr::setTimerOverflow() {
	
	SET_OVERFLOW(1);	
}

void Timerr::startTimer() {
	
	START_TIMERR(1,0,1);
	
}

void Timerr::stopTimer() {
	
	STOP_TIMER(1);
}

void Timerr::setCompareInterrupt() {
	SET_COMPARE(1, A);
}

void Timerr::resetTimer() {
	
	TCNT1 = 0;
	
}
void Timerr::startCustomTimer(int milli_sec) {
	
		int ocr = (15.625 * milli_sec);
		OCR1A = ocr;
		START_TIMERR(1,0,2); //for proteus use 5,0,1 and for real life use 5,0,2
}