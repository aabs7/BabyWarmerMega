/*
 * pins.h
 *
 * Created: 5/31/2019 2:16:45 AM
 *  Author: swain
 */ 


#ifndef PINS_H_
#define PINS_H_

#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */

//For Heater Control /
#define HEATER_PORT PORTB
#define HEATER_PIN PINB5

//For battery Monitor
#define BATT_MONITOR_PORT			PORTF
#define BATT_MONITOR_PIN			PINF
#define BATT_MONITOR_PIN_POSITION	PINF0

//Buzzer
#define BUZZER_PORT PORTH
#define BUZZER_PIN PINH4

//LED

#define SENSOR_FAIL_PORT PORTE //
#define SENSOR_FAIL_PIN PINE3     

#define HEATER_FAIL_PORT PORTH
#define HEATER_FAIL_PIN PINH3

#define TS_HIGH_PORT PORTL
#define TS_HIGH_PIN PINL1

#define TA_HIGH_PORT PORTL
#define TA_HIGH_PIN PINL5

#define POWER_PORT PORTG //
#define POWER_PIN PING5

#define SET_TEMP_PORT PORTL  //
#define SET_TEMP_PIN PINL3


//BUTTONS
#define SET_UP_BUTTON_PORT  PINB
#define SET_UP_BUTTON_PIN PINB4

#define SET_BUTTON_PORT PINH
#define SET_BUTTON_PIN PINH6

#define SET_DOWN_BUTTON_PORT PINH
#define SET_DOWN_BUTTON_PIN PINH5

#define BUZZER_STOP_BUTTON_PORT PINE
#define BUZZER_STOP_BUTTON_PIN PINE5


//SPI SLAVE SELECTS
#define DISPLAY_I_PORT PORTK
#define DISPLAY_I_PIN PINK7

#define DISPLAY_II_PORT PORTK
#define DISPLAY_II_PIN PINK6

#define TEMP_I_PORT PORTL
#define TEMP_I_PIN PINL0

#define TEMP_II_PORT PORTL
#define TEMP_II_PIN PINL2

//SPI PINS
#define MOSI_PORT	PORTB
#define SCK_PORT	PORTB
#define MISO_PORT	PORTB
#define SS_PORT		PORTB

#define SCK_PIN    PINB1
#define MOSI_PIN   PINB2
#define MISO_PIN   PINB3
#define SS_PIN    PINB0




#endif /* PINS_H_ */
