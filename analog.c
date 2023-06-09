/*
 * analog.c
 *
 * Created: 06.06.2023 18:00:43
 *  Author: Alexander Schroeder
 */ 

#include "analog.h"

#include <avr/interrupt.h>

uint32_t p = 0;

ISR(ADC_vect)
{
	p -= p >> 7;
	p += ADC;
}

void analog_init(void)
{
	ADMUX =  0b11000000;
	ADCSRA = 0b11111111;
}

int16_t analog_get_suc_pressure(void)
{
	int16_t result = p >> 7;
	return result*10 - 3490;  // 18bar max; 4-20mA sensor; 2,56V ref; 6,79bar max; R290 -> 12�C max
}

int16_t analog_get_suc_temp(void)
{
	int32_t result = analog_get_suc_pressure();
	result = -result*result*46/10000000 + result*113/1000 - 366;  // R290 tuned for -10C to +15C
	
	return result;
}