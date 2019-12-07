/*
 * spi.h
 *
 * Created: 5/18/2019 1:05:10 PM
 *  Author: swain
 */ 


#ifndef SPI_H_
#define SPI_H_


#include "pins.h"

//#define MAX7219_LOAD_OFF_1(num)          PORTB |= (1<<(PIN_SS ## num))
//#define MAX7219_LOAD_ON_1(num)          PORTB &=~ (1<<(PIN_SS ## num))

#define MAX7219_LOAD_OFF_1(num)          DISPLAY_I_PORT |= (1<<(DISPLAY_I_PIN))
#define MAX7219_LOAD_ON_1(num)          DISPLAY_I_PORT &=~ (1<<(DISPLAY_I_PIN))

#define MAX7219_LOAD_OFF_2(num)          DISPLAY_II_PORT |= (1<<(DISPLAY_II_PIN))
#define MAX7219_LOAD_ON_2(num)          DISPLAY_II_PORT &=~ (1<<(DISPLAY_II_PIN))

#include <avr/io.h>

void spi_init() {
	// SPI Enable, Master mode
	SPCR |= (1 << SPE) | (1 << MSTR)| (1<<SPR1);
	SPCR &=~ ((1 << SPR0) | (1<< CPHA));
	
	// SCK MOSI CS/LOAD/SS
	DDR(SCK_PORT) |= (1 << SCK_PIN) | (1 << MOSI_PIN) ;
	DDR(SS_PORT) |= (1  << SS_PIN);
	SS_PORT |= (1 << SS_PIN);
	
	//slave pins
	DDR(DISPLAY_I_PORT) |= (1 << DISPLAY_I_PIN) | (1 << DISPLAY_II_PIN);
	
	DISPLAY_I_PORT |= (1 << DISPLAY_I_PIN); 
	DISPLAY_II_PORT |= (1 << DISPLAY_II_PIN);

	
}

void spiSendByte (char databyte)
{
	// Copy data into the SPI data register
	SPDR = databyte;
	// Wait until transfer is complete
	while (!(SPSR & (1 << SPIF)));
}



#endif /* SPI_H_ */