/*
 * Warmer_V1_PID.cpp
 *
 * Created: 2/9/2020 11:42:18 AM
 * Author : chand
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "PID.h"
#include "Timerr.h"
#include "pins.h"
#include "uart.h"
#include "max31865.h"
#include "max7219.h"
#include "led.h"
#include "PWM.h"
#include "Controls.h"
#include "General_Functions.h"
#include "General-function.h"

void init_devices();
void check();
void sendToDisplay(float air, float skin, float set);
void displayOnTempOutOfRange(float air, float skin, float set);
void sendToDisplayBelow(float temp);
void displaySensFail();
void displayPreHeat();
void displaySetTemperature();
void titititi();
void titi();
void displayPwrFail();
void displaySkinAirSet();
void displayHighTemp();
void displayLowTemp();
void displayHighAir();
void displayBABY();
void checkTemperature();

#define BAUD 9600
#define BUFF_LEN 700
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define AIR_SENSOR 0
#define SKIN_SENSOR 1

//Zero Crossing 
#define External_Interrupt_vect4 INT4_vect

volatile bool Trigger_FLAG    = false;

//variables for firing 
int MAX_FIRING_DELAY    = 4999;
int FIRING_DELAY        = 4999;


void(* resetFunc) (void) = 0;
//monitoring for
bool startMonitor = false;
int second = 0;

bool displayOnTempOutOfRangeFlag = false;

//TIMER FOR DISPLAY UPDATE
Timerr timerr;
volatile bool displayUpdate = false;
volatile uint8_t spiTimerCheck = 0;


//BUZZER
Controls controls;
bool start_buzzer  = false;
int buzzer_count   = 0;
bool buzzer_timer  = false;

//PT100 Temperature
Adafruit_MAX31865 maxAir;
Adafruit_MAX31865 maxSkin;

volatile float skin_temperature = 0.0;
volatile float air_temperature  = 0.0;
float set_temperature           = 30.0;
float pre_heat_max_temp         = 30.0;

volatile int display_count      = 0;
volatile int display_count_max  = 1;

//Seven Segment Display
max7219   max1;
max7219   max2;

//LED
led Led;

//Buttons
bool set_up_pressed			= false;
bool set_pressed			= false;
bool set_down_pressed		= false;
bool buzzer_stop_pressed	= false;
bool stop_buzzer			= false;
bool button_change			= false;

bool low_temperature						= false;
bool high_temperature						= false;
bool air_high_temperature					= false;
bool baby_placed_and_setpoint_once_reached  = false;

//preheat heater
bool preHeat				= false;

//PID Object
PID Firing_PID;
volatile float Firing_Angle  = 0.0;

//PWM
PWM pwm;

int main(void)
{
	
	initUART0();
	maxSkin.begin(SKIN_SENSOR);
	init_devices();
	
//	max1.MAX7219_clearDisplay();
//	max2.MAX7219_clearDisplay();
//	displayBABY();
	
//	titititi();
	//sei();
	_delay_ms(10);
	
// 	max1.MAX7219_clearDisplay();
// 	max2.MAX7219_clearDisplay();
	
    while (1) 
    {
		if (displayUpdate)
		{
			//maxAir.begin(AIR_SENSOR);
			
			skin_temperature = maxSkin.temperature(100.0, 430.0);
			//air_temperature  = maxAir.temperature(100.0, 430.0);
// 			max1.MAX7219_init(REINITIALIZE);
// 			max2.MAX7219_init(REINITIALIZE);
			
			if (Firing_PID.PID_Flag)
			{
				Firing_Angle = Firing_PID.Compute_PID(skin_temperature);
				UART0TransmitData(Firing_Angle);
				UART0TransmitString("\t");
				UART0TransmitFloat(skin_temperature);
				UART0TransmitString("\t");
				Firing_PID.PID_Flag = false;
			}
			
			if (Trigger_FLAG)
			{
				pwm.SetOcrValue(Firing_Angle);                       //either (FIRING_DELAY - Firing_Angle) OR (Firing_Angle) only   
			//	UART0TransmitString("\t");
				UART0TransmitData(FIRING_DELAY - Firing_Angle);
				UART0TransmitString("\r\n");
				Trigger_FLAG = false;
			}
			displayUpdate = false;
 		}		
		else
		{
			asm volatile ("nop");
		}
    }
}

//***************************************************************************************************************************************************************//
//*****************************************************************************FUNCTION SECTION******************************************************************//
//***************************************************************************************************************************************************************//


void init_devices() 
{
	sei();
	
	//heater and buzzer
	controls.initControls();

	//led
	Led.led_init();
	
	//for time
	timerr.setTimerNum(2);
	timerr.setCompareInterrupt();
	timerr.startCustomTimer(200);
	//Initialization of PWM 50Hz signal
	pwm.InitPWM();
	
	//Initialization of External Interrupt for zero crossing detection
	Initialize_External_Interrupt_4();
	
	//PID Initialization
	Firing_PID.Initialize();
	InitializePIDTimer();
	Firing_PID.Set_Range(0, 4999);
	Firing_PID.SetSetPoint(set_temperature);
	Firing_PID.Set_PID(1.4545, 2.343, 0.005);
	
// 	max1.MAX7219_set(0,4,4);
// 	_delay_ms(10);
// 	max2.MAX7219_set(1, 4, 4);
// 	_delay_ms(10);
// 	

//	max1.MAX7219_init(FIRSTINITIALIZE);
// 	_delay_ms(10);
// 	max2.MAX7219_init(FIRSTINITIALIZE);
// 	_delay_ms(10);
// 	//reinitialize in case of spi hang
// 	max1.MAX7219_init(FIRSTINITIALIZE);
// 	_delay_ms(10);
// 	max2.MAX7219_init(FIRSTINITIALIZE);
// 	_delay_ms(10);
// 	//Clearing Display
// 	max1.MAX7219_clearDisplay();
// 	_delay_ms(30);
// 	max2.MAX7219_clearDisplay();
// 	_delay_ms(30);
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

void displaySetTemperature()
{
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

void displayPreHeat()
{
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

void displayHighTemp()
{
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

//*******************************************************************************************************************************************************************//
//**************************************************************************ISR SECTION******************************************************************************//
//*******************************************************************************************************************************************************************//

//Display Update and spiTimer

ISR(TIMER2_COMPA_vect)
{
	TCNT2 = 0;
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

//ISR For PID Computation
ISR(TIMER3_COMPA_vect)
{
	Firing_PID.PID_Flag = true;
}

//Zero Crossing Detection ISR
ISR(External_Interrupt_vect4)
{
	Trigger_FLAG = true;
}
