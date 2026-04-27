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
#include <stdlib.h>
#include "My_Libraries/UART.h"
/****************************************/
// Function prototypes
void initADC();

volatile uint8_t NIBBLE_H;
volatile uint8_t NIBBLE_L;
volatile uint8_t ASCII;
volatile uint8_t POT;
char buffer[33];
/****************************************/
// Main Function
int main(void)
{
	cli();
	initUART();
	initADC();
	writeString("Seleccionar opción: 1:Leer potenciómetro. 2:enviar ASCII.");
	DDRC	= 0x00;
	ADCSRA		|= (1<<ADSC);
	sei();
	while (1)
	{
		PORTB = NIBBLE_H;
		PORTD = (PORTD & 0x0F) | (NIBBLE_L << 4);
	}
}
/****************************************/
// NON-Interrupt subroutines

void initADC()
{
	//Aref = AVcc; Izquierda; ADC6
	ADMUX	= 0;
	ADMUX	|= (1<<REFS0) |  (1<<ADLAR) | (1<<MUX2) | (1<<MUX1);
	//Habilitar ADC y prescaler de 16
	ADCSRA	= (1<<ADEN) | (1<<ADPS2) | (1<<ADIE);
}

/****************************************/
// Interrupt routines
ISR(USART_RX_vect)
{
	char ValorRX = UDR0;
	if (ASCII==1)
	{
		NIBBLE_H = ((0xF0 & ValorRX)>>4); //bit 4 a 7
		NIBBLE_L = (0x0F & ValorRX);//bit 0 a 3
		ASCII = 0;
		writeString("Seleccionar opción: 1:Leer potenciómetro. 2:enviar ASCII.");
	} 
	else
	{
		if (ValorRX == 49)
		{
			itoa(POT, buffer, 10);
			writeString(buffer);
			writeString("Seleccionar opción: 1:Leer potenciómetro. 2:enviar ASCII.");
		} else if (ValorRX == 50)
		{
			writeString("Ingrese su caracter");
			ASCII = 1;
		}else
		{
			writeString("Seleccionar opción: 1:Leer potenciómetro. 2:enviar ASCII.");
		}
	}
}
ISR(ADC_vect)
{
	POT = ADCH;
	ADCSRA		|= (1<<ADSC);		//Inicia lectura ADC
}