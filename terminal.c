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
	UCSRA = 0b10;
	UCSRB = 0b11000;
	UCSRC = 0b10000110;
	UBRRH = 0b00000000;
	UBRRL = 16;
	
	stdout = &mystdout;
}

 int usart_putchar_printf(char var, FILE *stream)
 {
	 while(!(UCSRA & (1 << UDRE)));
	 UCSRA |= 1 << UDRE;
	 
	 UDR = var;
	 
	 return 0;
 }