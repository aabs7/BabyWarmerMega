#include "uart.h"

char timeOut=0;

char buffer=0,buffer1=0,buffer2=0;

void initUART0(void)
{
	//set baud rate
	UBRR0H=(MYUBRR0)>>8;
	UBRR0L=MYUBRR0;

	//Enable Receiver and Transmitter
	UCSR0B = ((1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0));
	
	//set frame format
	UCSR0C=((1<<UCSZ00)|(1<<UCSZ01));
}



void UART0Transmit(unsigned char data)
{
	//wait for transmit buffer to be empty
	while(!((UCSR0A)&(1<<UDRE0)));
	
	//send data
	UDR0=data;
}

unsigned char UART0Receive(void)
{
	char data=buffer;
	buffer=0;
	return data;
}



void UART0TransmitData(int data)
{
	char buffer[20];
	char *i=0;
	itoa(data,buffer,10);
	i=buffer;
	while(*i)
	UART0Transmit(*i++);
}

void UART0TransmitString(const char *s)
{
	while(*s)
	UART0Transmit(*s++);
}

ISR(USART0_RX_vect)
{
	buffer=UDR0;
	
}
