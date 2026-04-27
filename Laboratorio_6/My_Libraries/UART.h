/*
 * UART.h
 *
 * Created: 22/04/2026 16:18:41
 *  Author: Diego
 */ 

#ifndef UART_H_
#define UART_H_
#include <avr/io.h>
void initUART();
void writeChar(char caracter);
void writeString(char* string);


#endif /* UART_H_ */