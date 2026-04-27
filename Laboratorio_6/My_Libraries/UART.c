/*
 * UART.c
 *
 * Created: 22/04/2026 16:19:00
 *  Author: Diego
 */ 
#include "UART.h"
void writeChar(char caracter)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = caracter;
}
void writeString(char* string)
{
	for (uint8_t i=0; string[i] != '\0'; i++)
	{
		writeChar(string[i]);
	}
}
void initUART()
{
	//Configurar RX (PD0) y TX (PD1)
	DDRD	&= ~(1<<DDD0);
	DDRD	|= (1<<DDD1) | (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7);
	DDRB	= 0xFF;
	//Sin doble velocidad ni multiprocesador
	UCSR0A	= 0;
	//Interrupciones, transmisión y recepción habilitadas
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) |(1<<TXEN0);
	//8 bits
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	//Baud Rate de 2400
	UBRR0 = 416;
}