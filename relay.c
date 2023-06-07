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

#define PUMPS_ON		PORTD |= 1 << PD6
#define PUMPS_OFF		PORTD &= ~(1 << PD6)

enum comp_state_t { cs_init = 0, cs_idle, cs_softstart, cs_decay, cs_start, cs_stop } comp_state;
	
uint64_t relay_timer = 0;

void relay_init(void)
{
 	COMP_OFF;
 	SOFT_OFF;
	ICR1 = 512;
	TCCR1A = 0b11110010;  // COM1A and COM1B inverting
	TCCR1B = 0b11001;  // 9bit fast PWM, no prescaling -> 23.4375kHz

	PORTB &= ~((1 << PB1) | (1 << PB2));
	DDRB |= (1 << PB1) | (1 << PB2);
	
	PUMPS_OFF;
	DDRD |= 1 << PD6;
}

void relay_comp(uint8_t state)
{
	if (state == RELAY_DECAY)
		COMP_DECAY;
	else if (state == RELAY_ON)
		COMP_ON;
	else
		COMP_OFF;
}

void relay_soft(bool state)
{
	if (state)
		SOFT_ON;
	else
		SOFT_OFF;
}

void relay_pumps(bool state)
{
	if (state)
		PUMPS_ON;
	else
		PUMPS_OFF;
}
