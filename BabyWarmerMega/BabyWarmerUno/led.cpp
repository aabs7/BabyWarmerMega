#include "led.h"
#include "pins.h"
#include "header.h"
#include <avr/io.h>

#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */

void led::led_init() {
	DDR(TS_HIGH_PORT) |= (1 << TS_HIGH_PIN);
	DDR(TA_HIGH_PORT) |= (1 << TA_HIGH_PIN);
	DDR(SET_TEMP_PORT) |= (1 << SET_TEMP_PIN);
	DDR(SENSOR_FAIL_PORT) |= (1 << SENSOR_FAIL_PIN);
	DDR(HEATER_FAIL_PORT) |= (1 << HEATER_FAIL_PIN);
	DDR(POWER_PORT) |= (1 << POWER_PIN);
	
}


void led::led_do(int led_num, int state) {
	switch(led_num) {
		case TS_HIGH_LED:
		REGISTER_LED(TS_HIGH_PORT, TS_HIGH_PIN, state);
		break;
	
		case TA_HIGH_LED:
		REGISTER_LED(TA_HIGH_PORT, TA_HIGH_PIN, state);
		break;
		
		case SENSOR_FAIL_LED:
		REGISTER_LED(SENSOR_FAIL_PORT, SENSOR_FAIL_PIN, state);
		break;
		
		case HEATER_FAIL_LED:
		REGISTER_LED(HEATER_FAIL_PORT, HEATER_FAIL_PIN, state);
		break;
		
		case SET_TEMP_LED:
		REGISTER_LED(SET_TEMP_PORT, SET_TEMP_PIN, state);
		break;
		
		case POWER_LED:
		REGISTER_LED(POWER_PORT, POWER_PIN, state);
		break;

		
	}
	
}

void led::led_all(bool state) {
	REGISTER_LED(TS_HIGH_PORT, TS_HIGH_PIN, state);
	REGISTER_LED(TA_HIGH_PORT, TA_HIGH_PIN, state);
	REGISTER_LED(SENSOR_FAIL_PORT, SENSOR_FAIL_PIN, state);
	REGISTER_LED(HEATER_FAIL_PORT, HEATER_FAIL_PIN, state);
	REGISTER_LED(SET_TEMP_PORT, SET_TEMP_PIN, state);
	REGISTER_LED(POWER_PORT, POWER_PIN, state);
}