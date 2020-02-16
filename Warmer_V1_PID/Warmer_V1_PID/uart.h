/*
 * uart.h
 *
 * Created: 6/6/2019 8:02:32 AM
 *  Author: swain
 */ 

#ifndef UART_H_
#define UART_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif


#define BAUD_RATE0 9600
#define MYUBRR0 F_CPU/16/BAUD_RATE0-1

#define TIMEOUT 10

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>


char *float2str(double float_num);
void initUART0(void);
void UART0Transmit(unsigned char data);
unsigned char UART0Receive(void);
void UART0TransmitData(int data);
void UART0TransmitString(const char *s);
void UART0TransmitFloat(float num);

#endif /* UART_H_ */