/*
 * Timerr.h
 *
 * Created: 6/6/2019 8:04:29 AM
 *  Author: swain
 */ 

#ifndef TIMERR_H_
#define TIMERR_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define SET_OVERFLOW(num) TIMSK ## num |= _BV(TOIE ## num)
#define SET_COMPARE(num, let)  TIMSK ## num |= _BV(OCIE ## num ## let)
#define START_TIMER(num, num1) TCCR ## num ## B |= _BV(CS ## num ## num1)
#define START_TIMERR(num, num1, num2) TCCR ## num ## B |= (_BV(CS ## num ## num1) | _BV(CS ## num ## num2))
#define START_TIMERRR(num, num1, num2, num3) TCCR ## num ## B |= _BV(CS ## num ## num1)  | _BV(CS ## num ## num2)  | _BV(CS ## num ## num3)

#define STOP_TIMER(num) TCCR ## num ## B &=~ (_BV(CS ## num ## 0)|_BV(CS ## num ## 1) | _BV(CS ## num ## 2))


class Timerr {
	private:
	int timer_no;
	
	public:
	void setTimerNum(int timer_num);
	void setTimerOverflow();
	void setCompareInterrupt();
	void startTimer();
	void stopTimer();
	void startCustomTimer(int milli_sec);
	void resetTimer();
};



#endif /* DISPLAY_H_ */