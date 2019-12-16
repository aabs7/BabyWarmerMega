/*
 * max7219.h
 *
 * Created: 5/18/2019 1:08:31 PM
 *  Author: swain
 */ 


#ifndef MAX7219_H_
#define MAX7219_H_


#define ON 1
#define OFF 0
#define MAX7219_MODE_DECODE       0x09
#define MAX7219_MODE_INTENSITY    0x0A
#define MAX7219_MODE_SCAN_LIMIT   0x0B
#define MAX7219_MODE_POWER        0x0C
#define MAX7219_MODE_TEST         0x0F
#define MAX7219_MODE_NOOP         0x00

// I only have 3 digits, no point having the
// rest. You could use more though.
#define MAX7219_DIGIT0            0x01
#define MAX7219_DIGIT1            0x02
#define MAX7219_DIGIT2            0x03
#define MAX7219_DIGIT3            0x04

#define MAX7219_CHAR_BLANK        0xF
#define MAX7219_CHAR_NEGATIVE     0xA

/////////////////////////////////////////////////////////////
//Define characters.
//similarly to display hi.t, lo.t
#define H  0b00110111
#define idot  0b10000110
#define t  0b00001111
#define L  0b00001110
#define odot  0b11111110

//similarly to display SEt
#define S  0b01011011
#define E  0b01001111
#define SMALL_E	0b01101111
//t already defined

//define das (no output)
#define das 0b00000000

//define FAIL SENS
#define F 0b01000111
#define A 0b01110111
#define SMALL_A	0b01111101
// A already defined
#define I 0b00000110
// L already defined
// S already defined
// E already defined
#define N 0b00010101
// S already defined

//define pre heat
//define PRE
#define P 0b01100111
#define R 0b01000110
//E already defined

// define HEAT
// H already defined
// E already defined
// A already defined
// t already defined

//define ON and OFF
#define O 0b01111110
// n already defined
// F already defined
////////////////////////////////////////////////////////////////////////
#define B 0b01111111
#define Y 0b00111011

#define G 0b01011111

#define FIRSTINITIALIZE 0
#define REINITIALIZE	1


class max7219 {
		private:
			int num = 0;
			int digits1;
			int digits2;
		public:
			void MAX7219_set(int numm, int ii, int jj);
			void MAX7219_init(char a);
			void MAX7219_writeData(char data_register, char data);
			void MAX7219_clearDisplay();
			void MAX7219_displayNumber(volatile long number);
			
	};



#endif /* MAX7219_H_ */