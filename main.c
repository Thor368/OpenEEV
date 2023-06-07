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
#include <stdlib.h>

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

ISR(TIMER0_OVF_vect)
{
	ticks++;
}

void init(void)
{
	TCCR0 = 0b011;  // /64 prescaling
	TIMSK = 0b1;  // activate OCR0A interrupt
	
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
			
// 			DS_convert_T(temp_avaporator_adr);
// 			while(DS_bus_busy());
// 			int16_t temp = DS_read_temperature(temp_avaporator_adr);
// 			printf("temp = %d.%dC\n", temp/10, abs(temp%10));
 			
//  			uint8_t tmp[8];
// 			uint8_t crc = DS_read_ROM(tmp);
// 			
//  			printf("ROM = ");
// 			for (uint8_t i = 0; i < 8; i++)
// 				printf("0x%02x ", tmp[i]);
//  			printf("\n");

//			printf("temp = %d\n", analog_get_suc_temp());
		}
    }
}

