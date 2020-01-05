/*
 * MasterBabyWarmer.cpp
 *
 * Created: 11/8/2019 12:45:14 PM
 *  Author: NIC
 */ 


/*
 * MasterIncubator.cpp
 *
 * Created: 6/6/2019 8:01:04 AM
 *  Author: swain
 */ 

/*
 * MasterBabyWarmer.cpp
 *
 * Created: 11/10/2019 10:05:04 AM
 *  Co-Author: Abhish Khanal
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Header.h"
#include "Timerr.h"
#include "Pins.h"
#include "Controls.h"
#include "uart.h"
#include "max31865.h"
#include "max7219.h"
#include "led.h"

#define BAUD 9600
#define BUFF_LEN 700
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)


#define AIR_SENSOR 0
#define SKIN_SENSOR 1


void init_devices();
void check();
void sendToDisplay(float air, float skin, float set);
void displayOnTempOutOfRange(float air, float skin, float set);
void sendToDisplayBelow(float temp);
void displaySensFail();
void displayPreHeat();
void displaySetTemperature();
void displayON();
void displayOFF();
void displayBABY();
void displayNANONANI();
void displayNIC();
void titititi();
void titi();
void displayPwrFail();
void displaySkinAirSet();
void displayHighTemp();
void displayLowTemp();
void displayHighAir();
void checkTemperature();


void(* resetFunc) (void) = 0;
//monitoring for
bool startMonitor = false;
int second = 0;

bool displayOnTempOutOfRangeFlag = false;

//TIMER FOR DISPLAY UPDATE
Timerr timerr;
volatile bool displayUpdate = false;
volatile uint8_t spiTimerCheck = 0;

//Sensor failure flags
volatile bool Skin_Sensor_failure_FLAG = false;
volatile bool Air_Sensor_failure_FLAG  = false;

//FAN HEATER BUZZER
Controls controls;
bool start_buzzer = false;
int buzzer_count = 0;
bool buzzer_timer = false;

//PT100 TEMPERATURE
Adafruit_MAX31865 maxAir;
Adafruit_MAX31865 maxSkin;

volatile float skin_temperature = 0.0;
volatile float air_temperature = 0.0;
float set_temperature = 36.5;
float set_temperature_copy = set_temperature;
float pre_heat_max_temp = 30.0;

volatile int display_count = 0;
volatile int display_count_max = 2;

//DISPLAY
max7219 max1;
max7219 max2;

//LED
led Led;

//buttons
bool set_up_pressed = false;
bool set_pressed = false;
bool set_down_pressed = false;
bool buzzer_stop_pressed = false;
bool stop_buzzer = false;
bool button_change = false;

bool baby_placed_and_setpoint_once_reached = false;

bool low_temperature = false;
bool high_temperature = false;
bool air_high_temperature = false;
//preheat heater
bool preHeat = false;

//nonstandard/checking variables
uint32_t countpreheat = 0;

int main(void)
{	
	initUART0();
	init_devices();
	
	//********************************************************************************************************************************************//
	//**************************************************************Display BABY BABY ************************************************************//
	//********************************************************************************************************************************************//
	 max1.MAX7219_clearDisplay();
	 max2.MAX7219_clearDisplay();
	 displayBABY();
	 //*******************************************************************************************************************************************//
	 _delay_ms(1000);
	 //**************************************************************Display NYANO NANI***********************************************************//
	 displayNANONANI();
	 _delay_ms(2000);
	
	
	//initialize local variables.
	//sendToDisplay(air_temperature,skin_temperature, set_temperature);
	
	float temperature_heater_off = 0.0;
	float temperature_heater_on = 0.0;
	//
	//wait while preheat condition is meet.
	bool clear_display = false;
	bool set_temp_display = false;
	
	//**********************************************************************************//
	//Check if the power is coming from battery or AC supply
	while(bit_is_clear(BATT_MONITOR_PIN,BATT_MONITOR_PIN_POSITION)){
		//then power is coming from battery.
		titi();_delay_ms(500);
		displayPwrFail();
	}
	
	titi();
	//*********************************************************************************//
	
	//check Sensor and display sensor fail if failure of sensor
 	max1.MAX7219_clearDisplay();
 	max2.MAX7219_clearDisplay();
 	maxSkin.begin(SKIN_SENSOR);
 	maxAir.begin(AIR_SENSOR);
 	skin_temperature = maxSkin.temperature(100.0,430.0);
 	air_temperature = maxAir.temperature(100.0,430.0);
	 
	//check the sensor conditions, loop the program if it doesn't detect the air sensor and skin sensor//
 	while(skin_temperature <= 0.0 || skin_temperature >= 50 ){
 		if(displayUpdate){
			displayUpdate = false;
			max1.MAX7219_init(REINITIALIZE);
			max2.MAX7219_init(REINITIALIZE);
			displaySensFail();
			maxSkin.begin(SKIN_SENSOR);
			skin_temperature = maxSkin.temperature(100.0,430.0);
		}
		else{
			asm volatile ("nop");
		}
 	}

 	while(air_temperature <= 0.0 || air_temperature >= 50){
 		if(displayUpdate){
			displayUpdate = false;
 			max1.MAX7219_init(REINITIALIZE);
 			max2.MAX7219_init(REINITIALIZE);
 			displaySensFail();
 			maxAir.begin(AIR_SENSOR);
 			air_temperature = maxAir.temperature(100.0,430.0);
 		}
 		else{
 			asm volatile ("nop");
 		}
 	}
	////////// The sensor detection ends here ///////////////////
	
	
	
	//*************************************************************//
	/// After this start preheat condition of baby warmer //////
 	while(!preHeat){
		 
		 Led.led_do(SENSOR_FAIL_LED, 0);
		 Led.led_do(POWER_LED, 0);
 		
 		if(displayUpdate){
 			clear_display = !clear_display;
 			
 			displayUpdate = false;
 			maxSkin.begin(SKIN_SENSOR);
 			skin_temperature = maxSkin.temperature(100.0,430.0);
 			max1.MAX7219_init(REINITIALIZE);
 			max2.MAX7219_init(REINITIALIZE);
 			displayPreHeat();
 			if(clear_display) {
 				max2.MAX7219_clearDisplay();
 				//max1.MAX7219_clearDisplay();
 				} else {
 				sendToDisplayBelow(skin_temperature);
 			}
 			if(skin_temperature >= pre_heat_max_temp) {
 				controls.stopHeater();
 				preHeat = true;
 				titititi();
 			}
 			else if(skin_temperature < pre_heat_max_temp) {
 				controls.startHeater();
 			}
 		}
 		
 		else{
 			asm volatile ("nop");
 		}
 		
 	}

	///The preheat condition of baby warmer ends here ///////////
	
	//clear display
	//note that for clearDisplay to clear screen, mode decode register should be in 0xff form or the display shows tttttttt 

	max1.MAX7219_clearDisplay();
	max2.MAX7219_clearDisplay();
	_delay_ms(100);
	
	while(1)
	{
		check();
		
		while (Skin_Sensor_failure_FLAG | Air_Sensor_failure_FLAG)
		{
			check();
			Led.led_do(HEATER_FAIL_LED, 0);
			controls.stopHeater();
			titititi();
		}
		
		if (!Skin_Sensor_failure_FLAG && !Air_Sensor_failure_FLAG)
		{
			Led.led_do(SENSOR_FAIL_LED, 0);
		}
		
		if(displayUpdate) {
			
			//spi for temperature pt100
			maxAir.begin(AIR_SENSOR); //these are init
			maxSkin.begin(SKIN_SENSOR);
			skin_temperature = maxSkin.temperature(100.0, 430.0);
			air_temperature = maxAir.temperature(100.0, 430.0);
			max1.MAX7219_init(REINITIALIZE); // these are init
			max2.MAX7219_init(REINITIALIZE);
			
			//buzzer karaying
			if(start_buzzer) {
				buzzer_timer = !buzzer_timer;
				if(buzzer_timer) {
					//  UART0TransmitString("On\r\n");
					controls.startBuzzer();
					} else {
					// UART0TransmitString("off\r\n");
					controls.stopBuzzer();
				}
			}
			// sending this
			if(button_change){
				set_temp_display = !set_temp_display;
				if(set_temp_display) {
					sendToDisplayBelow(set_temperature_copy);
					} else {
					max2.MAX7219_clearDisplay();
				}
				displaySetTemperature();
			}
			
			else if(start_buzzer){
				displayOnTempOutOfRange(air_temperature, skin_temperature, set_temperature);
			}
			else {
				sendToDisplay(air_temperature, skin_temperature, set_temperature);
			}
			
			temperature_heater_off = set_temperature - 0.2f;
			temperature_heater_on = set_temperature - 0.3f;
			
			if(skin_temperature >= temperature_heater_off) {
				Led.led_do(HEATER_FAIL_LED,0);
				controls.stopHeater();
			}else if(skin_temperature <= temperature_heater_on) {
				Led.led_do(HEATER_FAIL_LED,1);
				controls.startHeater();
			}
			
			if (air_temperature >= 39.00 )
			{
				controls.stopHeater();
				Led.led_do(HEATER_FAIL_LED, 0);
				titititi();
				//Led.led_do(HEATER_FAIL_LED, 0);
				displayHighAir();
			}
			
			displayUpdate = false;
		}
		else{
			asm volatile ("nop");
		}
	}
}



void init_devices() {
		
	sei();
	
	//heater and buzzer
	controls.initControls();

	//led
	Led.led_init();
	
	//for time
	timerr.setTimerNum(1);
	timerr.setCompareInterrupt();
	timerr.startCustomTimer(200);
	
	max1.MAX7219_set(0,4,4);
	_delay_ms(10);
	max2.MAX7219_set(1, 4, 4);
	_delay_ms(10);
	
	//UART0TransmitString("inside adking \r\n");
	max1.MAX7219_init(FIRSTINITIALIZE);
	_delay_ms(10);
	max2.MAX7219_init(FIRSTINITIALIZE);
	_delay_ms(10);
	//reinitialize in case of spi hang
	max1.MAX7219_init(FIRSTINITIALIZE);
	_delay_ms(10);
	max2.MAX7219_init(FIRSTINITIALIZE);
	_delay_ms(10);
	
	//UART0TransmitString("out of adking \r\n");
	
	max1.MAX7219_clearDisplay();
	_delay_ms(30);
	max2.MAX7219_clearDisplay();
	_delay_ms(30);
}

ISR(TIMER1_COMPA_vect) {
	TCNT1 = 0;
	spiTimerCheck++;
	if(startMonitor) {
		second++;
	}
	display_count++;
	if(display_count >= display_count_max) {
		displayUpdate = true;
		display_count = 0;
	}
}

void check() {
	
	
	if((skin_temperature > 37.2) || (skin_temperature < (set_temperature - 1.0f)) || (air_temperature>39.00) ) {
		checkTemperature();
		if(!startMonitor) {
			start_buzzer = true;
		}
		
		if(stop_buzzer && !startMonitor) {
			startMonitor = true;
			second = 0;
			stop_buzzer = false;
			start_buzzer = false;
		}
	}
	else {
		controls.stopBuzzer();
		start_buzzer = false;
		Led.led_do(TS_HIGH_LED, 0);
		startMonitor = false;
		second = 0;
	}
	
	//***************************************************************************************************************//
	//******************************************************Check for skin sensor failure****************************//
	if(skin_temperature <= 0.0 || skin_temperature >= 50 )
	{
		if(displayUpdate)
		{
			displayUpdate = false;
			max1.MAX7219_init(REINITIALIZE);
			max2.MAX7219_init(REINITIALIZE);
			displaySensFail();
			maxSkin.begin(SKIN_SENSOR);
			skin_temperature = maxSkin.temperature(100.0,430.0);
			
			Skin_Sensor_failure_FLAG = true;
		}
		else
		{
			asm volatile ("nop");
		}
	}
	else
	{
		Skin_Sensor_failure_FLAG = false;
	}
	
	//************************************************************************************************************************//
	//************************************************* Check for Air Sensor failure******************************************//
	if(air_temperature <= 0.0 || air_temperature >= 50 )
	{
		if(displayUpdate)
		{
			displayUpdate = false;
			max1.MAX7219_init(REINITIALIZE);
			max2.MAX7219_init(REINITIALIZE);
			displaySensFail();
			maxSkin.begin(AIR_SENSOR);
			air_temperature = maxAir.temperature(100.0,430.0);
			
			Air_Sensor_failure_FLAG = true;
		}
		else
		{
			asm volatile ("nop");
		}
	}
	else
	{
		Air_Sensor_failure_FLAG = false;
	}
	
//*************************************************************************************************************************//
	//At first when the baby is just placed, alarm is continued every 5 minutes, but once baby set point temperature is reached, alarm is continued every 2 minutes
	if(baby_placed_and_setpoint_once_reached){
		if(second >= 600) {	//timer prescaled to 200ms so 600 means 2 minutes.
			//why this below line needed when if startMonitor = false, and skin temperature still set_temperature + 0.2
			//controls.startBuzzer();
			startMonitor = false;
			second = 0;
		}
	}
	else{
		if(second >= 1500) {	//timer prescaled to 200ms so 1500 means 5 minutes.
			//why this below line needed when if startMonitor = false, and skin temperature still set_temperature + 0.2
			//controls.startBuzzer();
			startMonitor = false;
			second = 0;
		}
		if(skin_temperature >= set_temperature){
			baby_placed_and_setpoint_once_reached = true;
		}
	}
	
	//buzzer stop button
	if(bit_is_clear(BUZZER_STOP_BUTTON_PORT, BUZZER_STOP_BUTTON_PIN) && !buzzer_stop_pressed ) {
		controls.stopBuzzer();
		stop_buzzer = true;
		buzzer_stop_pressed = true;
	}
	
	else if(bit_is_set(BUZZER_STOP_BUTTON_PORT, BUZZER_STOP_BUTTON_PIN)) {
		buzzer_stop_pressed = false;
	}
	
	//set up button
	
	if(bit_is_clear(SET_UP_BUTTON_PORT, SET_UP_BUTTON_PIN) && !set_up_pressed) {
		
		if(button_change) {
			set_temperature_copy += 0.1f;
			if(set_temperature_copy >= 37.2){
				set_temperature_copy = 37.2;
			}
		}
		set_up_pressed = true;
	}
	
	else if(bit_is_set(SET_UP_BUTTON_PORT, SET_UP_BUTTON_PIN)) {
		set_up_pressed = false;
	}
	
	
	//set down button
	if(bit_is_clear(SET_DOWN_BUTTON_PORT, SET_DOWN_BUTTON_PIN) && !set_down_pressed) {
		
		if(button_change) {
			set_temperature_copy -= 0.1f;
			//limit set temperature below range
			if(set_temperature_copy <= 32.00){
				set_temperature_copy = 32.00;
			}
		}
		
		set_down_pressed= true;
	}
	
	else if(bit_is_set(SET_DOWN_BUTTON_PORT, SET_DOWN_BUTTON_PIN)) {
		set_down_pressed = false;
	}
	
	//set button
	if(bit_is_clear(SET_BUTTON_PORT, SET_BUTTON_PIN) && !set_pressed) {
		button_change = !button_change;
		if(!button_change) {
			set_temperature = set_temperature_copy;
			titi();
		}
		
		set_pressed = true;
	}
	
	else if(bit_is_set(SET_BUTTON_PORT, SET_BUTTON_PIN)) {
		set_pressed = false;
	}
	
}

void sendToDisplay(float air, float skin, float set) {
	max1.MAX7219_writeData(MAX7219_MODE_DECODE,0xF3);
	max1.MAX7219_writeData(3,das);
	max1.MAX7219_writeData(8, air / 10);
	max1.MAX7219_writeData(6, ((int)air % 10) | 0b10000000);
	
	max1.MAX7219_writeData(2, (int)(air * 10) % 10);
	
	max1.MAX7219_writeData(4,das);
	max1.MAX7219_writeData(7, skin / 10);
	max1.MAX7219_writeData(5, ((int)skin % 10) | 0b10000000);
	
	max1.MAX7219_writeData(1, (int)(skin * 10) % 10);
	
	max2.MAX7219_writeData(MAX7219_MODE_DECODE,0xA2);
	//max2.MAX7219_writeData(1, set / 10);
	
	//max2.MAX7219_writeData(5, (int)set % 10);
	//max2.MAX7219_writeData(4, (int)(set*10) % 10);
	max2.MAX7219_writeData(4,das);
	max2.MAX7219_writeData(6, (set / 10));
	
	max2.MAX7219_writeData(8, ((int)set % 10)| 0b10000000);
	max2.MAX7219_writeData(2, (int)(set*10) % 10);
}
void checkTemperature() {
	if(skin_temperature > 37.35){
		low_temperature = false;
		high_temperature = true;
		
		Led.led_do(TS_HIGH_LED, 1);
	}
	else if(skin_temperature < (set_temperature - 1.0f)){
		low_temperature = true;
		high_temperature = false;
	}
	else if(air_temperature >39.00){
		air_high_temperature = true;
		high_temperature = false;
		low_temperature = false;
		Led.led_do(TA_HIGH_LED, 1);
	} else {
		air_high_temperature = false;
		high_temperature = false;
		low_temperature = false;
		Led.led_do(TA_HIGH_LED, 0);
		Led.led_do(TS_HIGH_LED, 0);
	}
}

void displayOnTempOutOfRange(float air, float skin, float set){
	
	if(high_temperature) {
		displayOnTempOutOfRangeFlag?displayHighTemp():sendToDisplay(air,skin,set);
	} else if(low_temperature) {
		displayOnTempOutOfRangeFlag?displayLowTemp():sendToDisplay(air,skin,set);
	} else if(air_high_temperature) {
		displayOnTempOutOfRangeFlag?displayHighAir():sendToDisplay(air,skin,set);
	}
	displayOnTempOutOfRangeFlag = !displayOnTempOutOfRangeFlag;
}
void sendToDisplayBelow(float temp){
	max2.MAX7219_writeData(MAX7219_MODE_DECODE,0xA2);
	max2.MAX7219_writeData(4,das);
	max2.MAX7219_writeData(6, (temp / 10));
	max2.MAX7219_writeData(8, ((int)temp % 10)| 0b10000000);
	max2.MAX7219_writeData(2, (int)(temp*10) % 10);
}


void displaySensFail()
{
	
	max1.MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
	//first 4-7segment display from left to right
	max1.MAX7219_writeData(3,S);
	max1.MAX7219_writeData(7,E);
	max1.MAX7219_writeData(5,N);
	max1.MAX7219_writeData(1,S);
	
	//second 4-7 segment display from left to right
	max1.MAX7219_writeData(4,F);
	max1.MAX7219_writeData(8,A);
	max1.MAX7219_writeData(6,I);
	max1.MAX7219_writeData(2,L);
	
	Led.led_do(SENSOR_FAIL_LED, 1);
}

void displaySetTemperature(){
	max1.MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
	//first 4-7segment display from left to right
	max1.MAX7219_writeData(3,das);
	max1.MAX7219_writeData(7,S);
	max1.MAX7219_writeData(5,E);
	max1.MAX7219_writeData(1,t);
	
	//second 4-7 segment display from left to right
	max1.MAX7219_writeData(4,t);
	max1.MAX7219_writeData(8,P);
	max1.MAX7219_writeData(6,das);
	max1.MAX7219_writeData(2,das);
}

void displayPreHeat(){
	max1.MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
	//first 4-7segment display from left to right
	max1.MAX7219_writeData(3,das);
	max1.MAX7219_writeData(7,P);
	max1.MAX7219_writeData(5,R);
	max1.MAX7219_writeData(1,SMALL_E);
	
	//second 4-7 segment display from left to right
	max1.MAX7219_writeData(4,H);
	max1.MAX7219_writeData(8,SMALL_E);
	max1.MAX7219_writeData(6,SMALL_A);
	max1.MAX7219_writeData(2,t);
	
	Led.led_do(HEATER_FAIL_LED, 1);
}

void displayON(){
	max1.MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
	//first 4-7segment display from left to right
	max1.MAX7219_writeData(3,das);
	max1.MAX7219_writeData(7,O);
	max1.MAX7219_writeData(5,N);
	max1.MAX7219_writeData(1,das);
	
	//second 4-7 segment display from left to right
	max1.MAX7219_writeData(4,das);
	max1.MAX7219_writeData(8,O);
	max1.MAX7219_writeData(6,F);
	max1.MAX7219_writeData(2,F);
	
}

void displayOFF(){
	max1.MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
	//first 4-7segment display from left to right
	max1.MAX7219_writeData(3,das);
	max1.MAX7219_writeData(7,O);
	max1.MAX7219_writeData(5,N);
	max1.MAX7219_writeData(1,das);
	
	//second 4-7 segment display from left to right
	max1.MAX7219_writeData(4,das);
	max1.MAX7219_writeData(8,O);
	max1.MAX7219_writeData(6,F);
	max1.MAX7219_writeData(2,F);
}

void displayHighTemp(){
	max1.MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
	//first 4-7segment display from left to right
	max1.MAX7219_writeData(3,H);
	max1.MAX7219_writeData(7,I);
	max1.MAX7219_writeData(5,G);
	max1.MAX7219_writeData(1,H);
	
	//second 4-7 segment display from left to right
	max1.MAX7219_writeData(4,t);
	max1.MAX7219_writeData(8,P);
	max1.MAX7219_writeData(6,das);
	max1.MAX7219_writeData(2,das);
}

void displayLowTemp(){
		max1.MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
		//first 4-7segment display from left to right
		max1.MAX7219_writeData(3,das);
		max1.MAX7219_writeData(7,das);
		max1.MAX7219_writeData(5,L);
		max1.MAX7219_writeData(1,odot);
		
		//second 4-7 segment display from left to right
		max1.MAX7219_writeData(4,t);
		max1.MAX7219_writeData(8,P);
		max1.MAX7219_writeData(6,das);
		max1.MAX7219_writeData(2,das);
}

void displayHighAir(){
	max1.MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
	//first 4-7segment display from left to right
	max1.MAX7219_writeData(3,H);
	max1.MAX7219_writeData(7,I);
	max1.MAX7219_writeData(5,G);
	max1.MAX7219_writeData(1,H);
	
	//second 4-7 segment display from left to right
	max1.MAX7219_writeData(4,A);
	max1.MAX7219_writeData(8,I);
	max1.MAX7219_writeData(6,R);
	max1.MAX7219_writeData(2,das);
}

void displayBABY(){
	
	//first 4-7segment display from left to right
	max1.MAX7219_writeData(3,B);
	max1.MAX7219_writeData(7,A);
	max1.MAX7219_writeData(5,B);
	max1.MAX7219_writeData(1,Y);
	
	//second 4-7 segment display from left to right
	max1.MAX7219_writeData(4,B);
	max1.MAX7219_writeData(8,A);
	max1.MAX7219_writeData(6,B);
	max1.MAX7219_writeData(2,Y);
}


void displayNANONANI(){
	
	//first 4-7segment display from left to right
	max1.MAX7219_writeData(3,N);
	max1.MAX7219_writeData(7,A);
	max1.MAX7219_writeData(5,N);
	max1.MAX7219_writeData(1,O);
	
	//second 4-7 segment display from left to right
	max1.MAX7219_writeData(4,N);
	max1.MAX7219_writeData(8,A);
	max1.MAX7219_writeData(6,N);
	max1.MAX7219_writeData(2,I);
}
// Order of right and left 4-7 segment matrix registers.
// 3 7 5 1      4 8 6 2
// 4,6,8,2

void displayNIC()
{
	max1.MAX7219_writeData(4,N);
	max1.MAX7219_writeData(6,I);
	max1.MAX7219_writeData(8,C);
	max1.MAX7219_writeData(2,das);
}

void displayPwrFail(){
		max1.MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
		max1.MAX7219_writeData(3,das);
		max1.MAX7219_writeData(7,das);
		max1.MAX7219_writeData(5,P);
		max1.MAX7219_writeData(1,R);
		//second 4-7 segment display from left to right
		max1.MAX7219_writeData(4,F);
		max1.MAX7219_writeData(8,A);
		max1.MAX7219_writeData(6,I);
		max1.MAX7219_writeData(2,L);
		
		Led.led_do(POWER_LED, 1);
		Led.led_do(SENSOR_FAIL_LED, 0);
		Led.led_do(HEATER_FAIL_LED, 0);
}

void displaySkinAirSet(){
	max1.MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
	//first 4-7segment display from left to right
	max1.MAX7219_writeData(3,S);
	max1.MAX7219_writeData(7,H);
	max1.MAX7219_writeData(5,I);
	max1.MAX7219_writeData(1,N);
	
	//second 4-7 segment display from left to right
	max1.MAX7219_writeData(4,das);
	max1.MAX7219_writeData(8,A);
	max1.MAX7219_writeData(6,I);
	max1.MAX7219_writeData(2,R);
	
	max2.MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
	max2.MAX7219_writeData(1,das);
	max2.MAX7219_writeData(6,S);
	max2.MAX7219_writeData(8,E);
	max2.MAX7219_writeData(2,t);
}

void titititi(){
	controls.startBuzzer();
	_delay_ms(100);
	controls.stopBuzzer();
	_delay_ms(100);
	controls.startBuzzer();
	_delay_ms(100);
	controls.stopBuzzer();
	_delay_ms(300);
	controls.startBuzzer();
	_delay_ms(100);
	controls.stopBuzzer();
	_delay_ms(100);
	controls.startBuzzer();
	_delay_ms(100);
	controls.stopBuzzer();
}

void titi(){
	controls.startBuzzer();
	_delay_ms(100);
	controls.stopBuzzer();
	_delay_ms(100);
	controls.startBuzzer();
	_delay_ms(100);
	controls.stopBuzzer();
	_delay_ms(100);
}