/*
 * terminal.c
 *
 * Created: 06.06.2023 16:40:36
 *  Author: Alexander Schroeder
 */ 

#include "config.h"
#include "terminal.h"
#include "nv.h"

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int usart_putchar_printf(char var, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

uint8_t term_rx_buffer[40];
uint8_t term_rx_pointer = 0;

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
 
 void term_handler(void)
 {
	if (UCSRA & (1 << RXC))
	{
		if (UDR == '\n')
		{
			term_rx_buffer[term_rx_pointer] = 0;
			term_rx_pointer = 0;
			
			char* cmd = strtok((char*) term_rx_buffer, " ");
			if (strcmp(cmd, "call"))
				call = true;
			else if (strcmp(cmd, "nocall"))
				call = false;
			else if (strcmp(cmd, "reset_nv"))
				nv_reset();
			else if (strcmp(cmd, "set_sh_tc"))
			{
				char* tc = NULL;
				strtok(NULL, tc);
				sh_tc = atoi(tc);
				nv_update();
			}
			else if (strcmp(cmd, "set_sh_target"))
			{
				char* sp = NULL;
				strtok(NULL, sp);
				sh_setpoint = atoi(sp);
				nv_update();
			}
			else if (strcmp(cmd, "set_sh_hyst"))
			{
				char* hyst = NULL;
				strtok(NULL, hyst);
				sh_hysteresis = atoi(hyst);
				nv_update();
			}
		}
		else
			term_rx_buffer[term_rx_pointer++] = UDR;
	}
 }