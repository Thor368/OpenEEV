/*
 * OpenEEV.c
 *
 * Created: 04.06.2023 21:11:41
 * Author : Alexander Schroeder
 */ 

#include "config.h"
#include "EEV.h"
#include "oneWire.h"
#include "relay.h"
#include "terminal.h"
#include "analog.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

// EEV IO
#define EEV_PORT		PORTD
#define EEV_DDR			DDRD
#define EEV_A1			PD5
#define EEV_A2			PD3
#define EEV_B1			PD4
#define EEV_B2			PD2

#define EEV_CLOSE		false
#define EEV_OPEN		true

uint64_t ticks = 0;

uint64_t test_timer = 0;
bool test_state = false;

ISR(TIMER0_COMPA_vect)
{
	ticks++;
}

void init(void)
{
	OCR0A = 249;
	TCCR0A = 0b11;  // fast PWM with OCR0A as TOP
	TCCR0B = 0b1011;  // /64 prescaling
	TIMSK0 = 0b10;  // activate OCR0A interrupt
	
	ticks = 0;
	
	EEV_init();
	DS_init();
	term_init();
	analog_init();
	
	sei();
}

int main(void)
{
	init();
	
//	_delay_ms(1000);
	
// 	EEV_home();
// 	EEV_steps(500, EEV_OPEN);

	test_timer = ticks + 1000;
    while (1) 
    {
//		relay_handler();
		
		if (ticks >= test_timer)
		{
			test_timer += 1000;
			
			p = analog_read(0b1111);
			printf("p=%u\n", p);
		}
    }
}

