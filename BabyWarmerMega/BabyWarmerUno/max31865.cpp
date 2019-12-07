/*
 * max31865.cpp
 *
 * Created: 7/30/2019 8:08:36 AM
 *  Author: swain
 */ 

#include "max31865.h"

#include <stdlib.h>
#include "header.h"
#include <util/delay.h>


bool Adafruit_MAX31865::begin(uint8_t choose1) {
	
	DISPLAY_I_PORT |= (1 << DISPLAY_I_PIN) | (1 << DISPLAY_II_PIN);
	SPI.begin();
	SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE1));
	for (uint8_t i=0; i<16; i++) {
		// readRegister8(i);
	}
	choose = choose1;
	setWires(MAX31865_3WIRE);
	enableBias(false);
	autoConvert(false);
	clearFault();
	
	//Serial.print("config: "); Serial.println(readRegister8(MAX31856_CONFIG_REG), HEX);
	return true;
}


uint8_t Adafruit_MAX31865::readFault(void) {
	return readRegister8(MAX31856_FAULTSTAT_REG);
}

void Adafruit_MAX31865::clearFault(void) {
	uint8_t t = readRegister8(MAX31856_CONFIG_REG);
	t &= ~0x2C;
	t |= MAX31856_CONFIG_FAULTSTAT;
	writeRegister8(MAX31856_CONFIG_REG, t);
}

void Adafruit_MAX31865::enableBias(bool b) {
	uint8_t t = readRegister8(MAX31856_CONFIG_REG);
	if (b) {
		t |= MAX31856_CONFIG_BIAS;       // enable bias
	} else {
		t &= ~MAX31856_CONFIG_BIAS;       // disable bias
	}
	writeRegister8(MAX31856_CONFIG_REG, t);
}

void Adafruit_MAX31865::autoConvert(bool b) {
	uint8_t t = readRegister8(MAX31856_CONFIG_REG);
	if (b) {
		t |= MAX31856_CONFIG_MODEAUTO;       // enable autoconvert
	} else {
		t &= ~MAX31856_CONFIG_MODEAUTO;       // disable autoconvert
	}
	writeRegister8(MAX31856_CONFIG_REG, t);
}

void Adafruit_MAX31865::setWires(max31865_numwires_t wires ) {
	uint8_t t = readRegister8(MAX31856_CONFIG_REG);
	if (wires == MAX31865_3WIRE) {
		t |= MAX31856_CONFIG_3WIRE;
	} else {
		// 2 or 4 wire
		t &= ~MAX31856_CONFIG_3WIRE;
	}
	writeRegister8(MAX31856_CONFIG_REG, t);
}

float  Adafruit_MAX31865::temperature(float RTDnominal, float refResistor) {
	// http://www.analog.com/media/en/technical-documentation/application-notes/AN709_0.pdf

	float Z1, Z2, Z3, Z4, Rt, temp;

	Rt = readRTD();
	Rt /= 32768;
	Rt *= refResistor;
	
	// Serial.print("\nResistance: "); Serial.println(Rt, 8);

	Z1 = -RTD_A;
	Z2 = RTD_A * RTD_A - (4 * RTD_B);
	Z3 = (4 * RTD_B) / RTDnominal;
	Z4 = 2 * RTD_B;

	temp = Z2 + (Z3 * Rt);
	temp = (sqrt(temp) + Z1) / Z4;
	
	if (temp >= 0) return temp;

	// ugh.
	Rt /= RTDnominal;
	Rt *= 100;      // normalize to 100 ohm

	float rpoly = Rt;

	temp = -242.02;
	temp += 2.2228 * rpoly;
	rpoly *= Rt;  // square
	temp += 2.5859e-3 * rpoly;
	rpoly *= Rt;  // ^3
	temp -= 4.8260e-6 * rpoly;
	rpoly *= Rt;  // ^4
	temp -= 2.8183e-8 * rpoly;
	rpoly *= Rt;  // ^5
	temp += 1.5243e-10 * rpoly;

	return temp;
}

uint16_t Adafruit_MAX31865::readRTD (void) {
	clearFault();
	enableBias(true);
	_delay_ms(10);
	uint8_t t = readRegister8(MAX31856_CONFIG_REG);
	t |= MAX31856_CONFIG_1SHOT;
	writeRegister8(MAX31856_CONFIG_REG, t);
	_delay_ms(65);

	uint16_t rtd = readRegister16(MAX31856_RTDMSB_REG);

	// remove fault
	rtd >>= 1;

	return rtd;
}

/**********************************************/

uint8_t Adafruit_MAX31865::readRegister8(uint8_t addr) {
	
	
	uint8_t ret = 0;
	readRegisterN(addr, &ret, 1);
	
	return ret;
}

uint16_t Adafruit_MAX31865::readRegister16(uint8_t addr) {
uint8_t buffer[2] = {0, 0};
readRegisterN(addr, buffer, 2);

uint16_t ret = buffer[0];
ret <<= 8;
ret |=  buffer[1];

return ret;
}


void Adafruit_MAX31865::readRegisterN(uint8_t addr, uint8_t buffer[], uint8_t n) {
  
  
switch(choose) {
	case 0:
	TEMP_I_PORT &=~ (1 << TEMP_I_PIN);
	TEMP_II_PORT |= (1 << TEMP_II_PIN);
	break;
	case 1:
	TEMP_II_PORT &=~ (1 << TEMP_II_PIN);
	TEMP_I_PORT |= (1 << TEMP_I_PIN);
	break;
}
  addr &= 0x7F; // make sure top bit is not set

 
  spixfer(addr);

  //Serial.print("$"); Serial.print(addr, HEX); Serial.print(": ");
  while (n--) {
	  buffer[0] = spixfer(0xFF);
	  //Serial.print(" 0x"); Serial.print(buffer[0], HEX);
	  buffer++;
  }
  switch(choose) {
   case 0:
	TEMP_I_PORT |= (1 << TEMP_I_PIN);
	break;
  case 1:
	TEMP_II_PORT |= (1 << TEMP_II_PIN);
	break;
}

 
}


void Adafruit_MAX31865::writeRegister8(uint8_t addr, uint8_t data) {
 
  switch(choose) {
	  case 0:
		TEMP_I_PORT &=~ (1 << TEMP_I_PIN);
		TEMP_II_PORT |= (1 << TEMP_II_PIN);
		break;
	  case 1:
		TEMP_II_PORT &=~ (1 << TEMP_II_PIN);
		TEMP_I_PORT |= (1 << TEMP_I_PIN);
		break;
  }
  spixfer(addr | 0x80);   // make sure top bit is set
  spixfer(data);

  //Serial.print("$"); Serial.print(addr, HEX); Serial.print(" = 0x"); Serial.println(data, HEX);

switch(choose) {
   case 0:
	TEMP_I_PORT |= (1 << TEMP_I_PIN);
	break;
  case 1:
	TEMP_II_PORT |= (1 << TEMP_II_PIN);
	break;
}
}


uint8_t Adafruit_MAX31865::spixfer(uint8_t x) {
  
    return SPI.transfer(x);
}