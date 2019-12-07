
#ifndef HEADER_H_
#define HEADER_H_


#define INPUT2(port,pin) DDR ## port &= ~_BV(pin)
#define OUTPUT2(port,pin) DDR ## port |= _BV(pin)
#define CLEAR2(port,pin) PORT ## port &= ~_BV(pin)
#define SET2(port,pin) PORT ## port |= _BV(pin)
#define TOGGLE2(port,pin) PORT ## port ^= _BV(pin)
#define READ2(port,pin) ((PIN ## port & _BV(pin))?1:0)
#define REGISTER_SET1( REGISTER, BIT1 ) REGISTER|=_BV( BIT1 )
#define REGISTER_SET2( REGISTER, BIT1, BIT2 ) REGISTER|=_BV( BIT1 )|_BV( BIT2 )
#define REGISTER_SET3( REGISTER, BIT1, BIT2, BIT3 ) REGISTER|=_BV( BIT1 )|_BV( BIT2 )|_BV( BIT3 )
#define REGISTER_SET4( REGISTER, BIT1, BIT2, BIT3, BIT4 ) REGISTER|=_BV( BIT1 )|_BV( BIT2 )|_BV( BIT3 )|_BV( BIT4 )
#define REGISTER_SET5( REGISTER, BIT1, BIT2, BIT3, BIT4, BIT5 ) REGISTER|=_BV( BIT1 )|_BV( BIT2 )|_BV( BIT3 )|_BV( BIT4 )|_BV( BIT5 )
#define REGISTER_SET6( REGISTER, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6 ) REGISTER|=_BV( BIT1 )|_BV( BIT2 )|_BV( BIT3 )|_BV( BIT4 )|_BV( BIT5 )|_BV( BIT6 )
#define REGISTER_RESET( REGISTER,BIT ) REGISTER&=~_BV( BIT )
#define SET_ALL(port) PORT ## port = 0xff
#define RESET_ALL(port) PORT ## port = 0x00
#define OUTPUT_ALLDDRport) ( ## port = 0xff
#define INPUT_ALL(port) DDR ## port = 0x00

#define REGISTER_LED(REGISTER, BIT, STATE) STATE?REGISTER_SET1(REGISTER, BIT): REGISTER_RESET(REGISTER, BIT)
//
#define INPUT(x) INPUT2(x)
#define OUTPUT(x) OUTPUT2(x)
#define CLEAR(x) CLEAR2(x)
#define SET(x) SET2(x)
#define TOGGLE(x) TOGGLE2(x)n
#define READ(x) READ2(x)
#define PULLUP_ON(x) INPUT2(x); SET2(x)
#define PULLUP_OFF(x) INPUT2(x); CLEAR2(x)

#define _16bitTo8bit( _16BitNum, _8BitHigh, _8BitLow ) _8BitLow=_16BitNum;_8BitHigh=( _16BitNum>>8 );
#define _8bitTo16bit( _16BitNum, _8BitHigh, _8BitLow ) _16BitNum=(int)_8BitLow;_16BitNum|=( (int)_8BitHigh<<8 );

#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */



#define General_interrupt_enable() sei()


#endif /* HEADER_H_ */