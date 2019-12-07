/*
 * Controls.cpp
 *
 * Created: 6/6/2019 8:09:19 AM
 *  Author: swain
 */ 

#include "Controls.h"


void Controls::initControls(){
	//DDR(FAN_PORT) |= (1 << FAN_PIN);
	DDR(HEATER_PORT) |= (1 << HEATER_PIN);
	DDR(BUZZER_PORT) |= (1 << BUZZER_PIN);
	//FAN_PORT &=~ (1 << FAN_PIN);
	HEATER_PORT &=~ (1 << HEATER_PIN);
	//DDRB |= (1 << PINB7) | (1 << PINB4);
	
}

void Controls::startFan(){
	//FAN_PORT |= (1 << FAN_PIN);
	//PORTB |= (1 << PINB7) | (1 << PINB4);
}

void Controls::startHeater() {
	HEATER_PORT |= (1 << HEATER_PIN);
}

void Controls::stopFan() {
	//FAN_PORT &=~ (1 << FAN_PIN);
	//PORTB &=~ ((1 << PINB7) | (1 << PINB4));
}

void Controls::stopHeater() {
	HEATER_PORT &=~ (1 << HEATER_PIN);
}

void Controls::startBuzzer() {
	BUZZER_PORT |= (1 << BUZZER_PIN);
}

void Controls::stopBuzzer() {
	BUZZER_PORT &=~ (1 << BUZZER_PIN);
}