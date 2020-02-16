/*
 * Controls.cpp
 *
 * Created: 6/6/2019 8:09:19 AM
 *  Author: swain
 */ 

#include "Controls.h"


void Controls::initControls()
{
	DDR(TRIGGER_PORT) |= (1 << TRIGGER_PIN);
	DDR(BUZZER_PORT) |= (1 << BUZZER_PIN);

	TRIGGER_PORT &=~ (1 << TRIGGER_PIN);
	//battery monitor pin as input
	DDR(BATT_MONITOR_PORT) &= ~(1<<BATT_MONITOR_PIN_POSITION);
}


// void Controls::startHeater() {
// 	TRIGGER_PORT |= (1 << TRIGGER_PIN);
// }
// 
// 
// void Controls::stopHeater() {
// 	TRIGGER_PORT &=~ (1 << TRIGGER_PIN);
// }

void Controls::startBuzzer() 
{
	BUZZER_PORT |= (1 << BUZZER_PIN);
}

void Controls::stopBuzzer()
{
	BUZZER_PORT &=~ (1 << BUZZER_PIN);
}