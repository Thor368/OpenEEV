/*
 * relay.c
 *
 * Created: 06.06.2023 11:10:44
 *  Author: Alexander Schroeder
 */ 


#include "config.h"
#include "relay.h"

#include <avr/io.h>

#define COMP_OFF		OCR1A = 511  // 0V
#define COMP_ON			OCR1A = 255  // 12V
#define COMP_DECAY		OCR1A = 340  // 7V

#define SOFT_OFF		OCR1B = 511  // 0V
#define SOFT_ON			OCR1B = 255  // 12V

enum comp_state_t { cs_init = 0, cs_idle, cs_softstart, cs_decay, cs_start, cs_stop } comp_state;
	
uint64_t relay_timer = 0;

void relay_init(void)
{
 	COMP_OFF;
 	SOFT_OFF;
	TCCR1A = 0b11110010;  // COM1A and COM1B inverting
	TCCR1B = 0b1001;  // 9bit fast PWM, no prescaling -> 23.4375kHz
 	TCCR1C = 0;

	PORTB &= ~((1 << PB1) | (1 << PB2));
	DDRB |= (1 << PB1) | (1 << PB2);
}

void relay_comp_start(void)
{
	if (comp_state == cs_idle)
		comp_state = cs_start;
}

void relay_comp_stop(void)
{
	comp_state = cs_stop;
}

void relay_handler(void)
{
	switch(comp_state)
	{
		case cs_init:
			relay_init();
			
			comp_state = cs_idle;
			break;
		
		case cs_idle:
			break;
		
		case cs_softstart:
			if (ticks >= relay_timer)
			{
				COMP_ON;
				
				relay_timer = ticks + 200;
				comp_state = cs_decay;
			}
			break;

		case cs_decay:
			if (ticks >= relay_timer)
			{
				COMP_DECAY;
				SOFT_OFF;

				comp_state = cs_idle;
			}
			break;
				
		case cs_start:
			COMP_OFF;
			SOFT_ON;
			
			relay_timer = ticks + 2000;
			comp_state = cs_softstart;
			break;
		
		case cs_stop:
			COMP_OFF;
			SOFT_OFF;
			
			comp_state = cs_idle;
			break;
	}
}