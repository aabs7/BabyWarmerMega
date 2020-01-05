/*
 * max31865.h
 *
 * Created: 7/30/2019 8:08:17 AM
 *  Author: swain
 */ 


#ifndef MAX31865_H_
#define MAX31865_H_

#include <avr/io.h>
#include "spi.h"
#include "pins.h"
//#include "uart.h"



#define MAX31856_CONFIG_REG            0x00
#define MAX31856_CONFIG_BIAS           0x80
#define MAX31856_CONFIG_MODEAUTO       0x40
#define MAX31856_CONFIG_MODEOFF        0x00
#define MAX31856_CONFIG_1SHOT          0x20
#define MAX31856_CONFIG_3WIRE          0x10
#define MAX31856_CONFIG_24WIRE         0x00
#define MAX31856_CONFIG_FAULTSTAT      0x02
#define MAX31856_CONFIG_FILT50HZ       0x01
#define MAX31856_CONFIG_FILT60HZ       0x00

#define MAX31856_RTDMSB_REG           0x01
#define MAX31856_RTDLSB_REG           0x02
#define MAX31856_HFAULTMSB_REG        0x03
#define MAX31856_HFAULTLSB_REG        0x04
#define MAX31856_LFAULTMSB_REG        0x05
#define MAX31856_LFAULTLSB_REG        0x06
#define MAX31856_FAULTSTAT_REG        0x07


#define MAX31865_FAULT_HIGHTHRESH     0x80
#define MAX31865_FAULT_LOWTHRESH      0x40
#define MAX31865_FAULT_REFINLOW       0x20
#define MAX31865_FAULT_REFINHIGH      0x10
#define MAX31865_FAULT_RTDINLOW       0x08
#define MAX31865_FAULT_OVUV           0x04


#define RTD_A 3.9083e-3
#define RTD_B -5.775e-7

typedef enum max31865_numwires {
	MAX31865_2WIRE = 0,
	MAX31865_3WIRE = 1,
	MAX31865_4WIRE = 0
} max31865_numwires_t;


class Adafruit_MAX31865 {
	public:
	//SPI spi;
	bool begin(uint8_t choose = 0);

	uint8_t readFault(void);
	void clearFault(void);
	uint16_t readRTD();
	uint8_t choose;


	void setWires(max31865_numwires_t wires);
	void autoConvert(bool b);
	void enableBias(bool b);

	float temperature(float RTDnominal, float refResistor);

	
	int8_t _sclk, _miso, _mosi, _cs;

	void readRegisterN(uint8_t addr, uint8_t buffer[], uint8_t n);

	uint8_t  readRegister8(uint8_t addr);
	uint16_t readRegister16(uint8_t addr);

	void     writeRegister8(uint8_t addr, uint8_t reg);
	uint8_t spixfer(uint8_t addr);
};



#endif /* MAX31865_H_ */