/*
 * terminal.c
 *
 * Created: 06.06.2023 16:40:36
 *  Author: Alexander Schroeder
 */ 

#include "terminal.h"

#include <avr/io.h>
#include <stdio.h>

int usart_putchar_printf(char var, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

void term_init(void)
{
	UCSR0A = 0b10;
	UCSR0B = 0b11000;
	UCSR0C = 0b110;
	UBRR0 = 12;
	
	stdout = &mystdout;
}

 int usart_putchar_printf(char var, FILE *stream)
 {
	 while(!(UCSR0A & (1 << UDRE0)));
	 UCSR0A |= 1 << UDRE0;
	 
	 UDR0 = var;
	 
	 return 0;
 }