/*
 * EEV.c
 *
 * Created: 06.06.2023 08:32:04
 *  Author: Alexander Schroeder
 */ 

#include "config.h"
#include "EEV.h"

#include <avr/io.h>
#include <util/delay.h>

void EEV_init(void)
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

void EEV_steps(uint16_t steps, bool dir)
{
	EEV_step(false, true);

	for (uint16_t i = 0; i < steps; i++)
	EEV_step(dir, false);

	EEV_step(false, true);
}

void EEV_home(void)
{
	EEV_steps(700, EEV_CLOSE);
}
