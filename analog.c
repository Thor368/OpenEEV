/*
 * analog.c
 *
 * Created: 06.06.2023 18:00:43
 *  Author: Alexander Schroeder
 */ 

#include <avr/interrupt.h>

#include "analog.h"

uint16_t p = 0;

ISR(ADC_vect)
{
	p = ADC;
}

void analog_init(void)
{
	ADMUX =  0b01000000;
	ADCSRA = 0b11000001;
	
	while(ADCSRA & (1 << ADSC));
}

uint16_t analog_read(uint8_t channel)
{
	ADMUX = (ADMUX & 0xF0) | (channel & 0xF);
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return ADC;
}