
#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

#define OFF 0
#define ON 1

#define TS_HIGH_LED 0
#define TA_HIGH_LED 1
#define SET_TEMP_LED 2
#define POWER_LED 3
#define HEATER_FAIL_LED 4 
#define SENSOR_FAIL_LED 5


class led {
	public:
	void led_init();
	void led_do(int led_num, int state);
	void led_all(bool state);
};

#endif /* PINS_H_ */