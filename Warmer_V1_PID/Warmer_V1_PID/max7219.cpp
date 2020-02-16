/*
 * max7219.cpp
 *
 * Created: 5/18/2019 1:08:49 PM
 *  Author: swain
 */ 
#include "max7219.h"
#include "spiD.h"

void max7219::MAX7219_set(int numm, int ii, int jj) {
	num = numm;
	digits1 = ii;
	digits2 = jj;
}
void max7219::MAX7219_init(char a) {
	if(a == REINITIALIZE){
		spi_init();
	}
	else{
		spi_init();
		// Decode mode to "Font Code-B"
		max7219::MAX7219_writeData(MAX7219_MODE_DECODE, 0xFF);
		max7219::MAX7219_writeData(MAX7219_MODE_TEST, 0);

		// Scan limit runs from 0.
		max7219::MAX7219_writeData(MAX7219_MODE_SCAN_LIMIT, 7); //7 or 8?
		max7219::MAX7219_writeData(MAX7219_MODE_INTENSITY, 0xff);
		max7219::MAX7219_writeData(MAX7219_MODE_POWER, ON);
	}
}

void max7219::MAX7219_writeData(char data_register, char data)
{
	
	switch(max7219::num){
		case 0:
		MAX7219_LOAD_ON_1(0);
		break;
		case 1:
		MAX7219_LOAD_ON_2(1);
		break;
	}		
		
	// Send the register where the data will be stored
	spiSendByte(data_register);
	// Send the data to be stored
	spiSendByte(data);
	
	switch(max7219::num){
		case 0:
		MAX7219_LOAD_OFF_1(0);
		break;
		case 1:
		MAX7219_LOAD_OFF_2(1);
		break;
	}
	
}

void max7219::MAX7219_clearDisplay()
{
	max7219::MAX7219_writeData(MAX7219_MODE_DECODE,0x00);
	max7219::MAX7219_writeData(1,das);
	max7219::MAX7219_writeData(2,das);
	max7219::MAX7219_writeData(3,das);
	max7219::MAX7219_writeData(4,das);
	max7219::MAX7219_writeData(5,das);
	max7219::MAX7219_writeData(6,das);
	max7219::MAX7219_writeData(7,das);
	max7219::MAX7219_writeData(8,das);
	/*
	//char i = digitsInUse;
	char i = max7219::digits1 + max7219::digits2 ;
	// Loop until 0, but don't run for zero
	do {
		// Set each display in use to blank
		max7219::MAX7219_writeData(i, MAX7219_CHAR_BLANK);
	} while (--i);
	*/
}

void max7219::MAX7219_displayNumber(volatile long number)
{
	char negative = 0;

	// Convert negative to positive.
	// Keep a record that it was negative so we can
	// sign it again on the display.
	if (number < 0) {
		negative = 1;
		number *= -1;
	}

	max7219::MAX7219_clearDisplay();
	
	// If number = 0, only show one zero then exit
	if (number == 0) {
		max7219::MAX7219_writeData(MAX7219_DIGIT0, 0);
		max7219::MAX7219_writeData(MAX7219_DIGIT1, 0);
		max7219::MAX7219_writeData(MAX7219_DIGIT2, 0);
		max7219::MAX7219_writeData(MAX7219_DIGIT3, 0);
		return;
	}
	
	// Initialization to 0 required in this case,
	// does not work without it. Not sure why.
	//int i = digitsInUse;
	int i = max7219::digits1;
	// Loop until number is 0.
	do {
		max7219::MAX7219_writeData(i--, number % 10);
		max7219::MAX7219_writeData(i + 5, number % 10);
		// Actually divide by 10 now.
		number /= 10;
	} while (number != 0);
	
	// Bear in mind that if you only have three digits, and
	// try to display something like "-256" all that will display
	// will be "256" because it needs an extra fourth digit to
	// display the sign.
	if (negative) {
		max7219::MAX7219_writeData(i, MAX7219_CHAR_NEGATIVE);
	}
} 
