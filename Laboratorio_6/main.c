/*
 * NombreProgra.c
 *
 * Created: 19/04/26
 * Author: Diego Camey
 * Description: Laboratorio #6
 */
/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>
/****************************************/
// Function prototypes
void initUART();
void initADC();
void writeChar(char caracter);
void writeString(char* string);
void READPOT();

volatile uint8_t NIBBLE_H;
volatile uint8_t NIBBLE_L;
volatile uint8_t LECT;
/****************************************/
// Main Function
int main(void)
{
	cli();
	initUART();
	
	sei();
	while (1)
	{
		PORTB = NIBBLE_H;
		PORTD = (PORTD & 0x0F) | (NIBBLE_L << 4);
	}
}
/****************************************/
// NON-Interrupt subroutines
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
void initADC()
{
	//Aref = AVcc; Izquierda; ADC6
	ADMUX	= 0;
	ADMUX	|= (1<<REFS0) |  (1<<ADLAR) | (1<<MUX2) | (1<<MUX1);
	//Habilitar ADC y prescaler de 16 para 62.5 kHz
	ADCSRA	= (1<<ADEN) | (1<<ADPS2);
}
void READPOT()
{
	writeChar(ADCH);
}

/****************************************/
// Interrupt routines
ISR(USART_RX_vect)
{
	char bufferRX = UDR0;
	writeChar(bufferRX);
	NIBBLE_H = ((0xF0 & bufferRX)>>4); //bit 4 a 7
	NIBBLE_L = (0x0F & bufferRX);//bit 0 a 3
}
ISR(ADC_vect)
{
	ADCSRA		|= (1<<ADSC);		//Inicia lectura ADC
}