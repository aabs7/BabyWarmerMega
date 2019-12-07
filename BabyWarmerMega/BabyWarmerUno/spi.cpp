/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPI.h"

SPIClass SPI;


void SPIClass::begin()
{
  
    //pinMode(SS, OUTPUT);
	DDR(SS_PORT) |= (1 << SS_PIN);
    // Warning: if the SS pin ever becomes a LOW INPUT then SPI
    // automatically switches to Slave, so the data direction of
    // the SS pin MUST be kept as OUTPUT.
    SPCR |= _BV(MSTR);
    SPCR |= _BV(SPE);
	SPCR &=~ ((1 << SPR1) | (1 << SPR0));

    // Set direction register for SCK and MOSI pin.
    // MISO pin automatically overrides to INPUT.
    // By doing this AFTER enabling SPI, we avoid accidentally
    // clocking in a single bit since the lines go directly
    // from "input" to SPI control.
    // http://code.google.com/p/arduino/issues/detail?id=888
	
    //pinMode(SCK, OUTPUT);
	DDR(SCK_PORT) |= (1 << SCK_PIN);
    //pinMode(MOSI, OUTPUT);
	DDR(MOSI_PORT) |= (1 << MOSI_PIN);
	
	DDR(TEMP_I_PORT ) |= (1 << TEMP_I_PIN ) | (1 << TEMP_II_PIN );
	TEMP_I_PORT  |= (1 << TEMP_I_PIN ) | (1 << TEMP_II_PIN );
}
