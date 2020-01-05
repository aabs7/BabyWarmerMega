/*
 * Controls.h
 *
 * Created: 6/6/2019 8:09:31 AM
 *  Author: swain
 */ 

#ifndef CONTROLS_H_
#define CONTROLS_H_

#include <avr/io.h>
#include "pins.h"
#include "Header.h"

class Controls {
	public:
	void initControls();
	void startFan();
	void stopFan();
	void startHeater();
	void stopHeater();
	void startBuzzer();
	void stopBuzzer();
	
};




#endif /* CONTROLS_H_ */