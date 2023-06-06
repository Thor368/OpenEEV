/*
 * OpenEEV.c
 *
 * Created: 04.06.2023 21:11:41
 * Author : Alexander Schroeder
 */ 
#define F_CPU 12000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

// EEV IO
#define EEV_PORT		PORTD
#define EEV_DDR			DDRD
#define EEV_A1			PD5
#define EEV_A2			PD3
#define EEV_B1			PD4
#define EEV_B2			PD2

#define EEV_CLOSE		false
#define EEV_OPEN		true

void init(void)
{
	EEV_PORT &= ~((1 << EEV_A1) | (1 << EEV_A2) | (1 << EEV_B1) | (1 << EEV_B2));
	EEV_DDR |= (1 << EEV_A1) | (1 << EEV_A2) | (1 << EEV_B1) | (1 << EEV_B2);
	
}

void EEV_step(bool dir, bool hold_pulse)
{
	static uint8_t step = 0;
	
	if (hold_pulse) {}
	else if (dir)
	{
		if (step == 7)
			step = 0;
		else
			step++;
	}
	else
	{
		if (!step)
			step = 7;
		else
			step--;
	}
	
	EEV_PORT &= ~((1 << EEV_A1) | (1 << EEV_A2) | (1 << EEV_B1) | (1 << EEV_B2));
	switch (step)
	{
		case 0:
			EEV_PORT |= (1 << EEV_A1);
			break;

		case 1:
			EEV_PORT |= (1 << EEV_A1) | (1 << EEV_B1);
			break;

		case 2:
			EEV_PORT |= (1 << EEV_B1);
			break;

		case 3:
			EEV_PORT |= (1 << EEV_A2) | (1 << EEV_B1);
			break;

		case 4:
			EEV_PORT |= (1 << EEV_A2);
			break;

		case 5:
			EEV_PORT |= (1 << EEV_A2) | (1 << EEV_B2);
			break;

		case 6:
			EEV_PORT |= (1 << EEV_B2);
			break;

		case 7:
			EEV_PORT |= (1 << EEV_A1) | (1 << EEV_B2);
			break;
	}
	
	if (hold_pulse)
		_delay_ms(500);
	else
		_delay_ms(12);

	EEV_PORT &= ~((1 << EEV_A1) | (1 << EEV_A2) | (1 << EEV_B1) | (1 << EEV_B2));
}

int main(void)
{
	init();

	EEV_step(EEV_CLOSE, true);
	for (uint16_t i = 0; i < 700; i++)
		EEV_step(EEV_CLOSE, false);
	
	EEV_step(EEV_CLOSE, true);
	
    while (1) 
    {
		EEV_step(EEV_CLOSE, true);
		for (uint16_t i = 0; i < 500; i++)
			EEV_step(EEV_OPEN, false);
	
		EEV_step(EEV_CLOSE, true);
	
		_delay_ms(1000);

		EEV_step(EEV_CLOSE, true);
		for (uint16_t i = 0; i < 500; i++)
			EEV_step(EEV_CLOSE, false);
		
		EEV_step(EEV_CLOSE, true);
		
		_delay_ms(1000);
    }
}

