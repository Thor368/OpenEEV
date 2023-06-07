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
#include <stdlib.h>

uint8_t position = 0;


void EEV_init(void)
{
	EEV_PORT &= ~((1 << EEV_A1) | (1 << EEV_A2) | (1 << EEV_B1) | (1 << EEV_B2));
	EEV_DDR |= (1 << EEV_A1) | (1 << EEV_A2) | (1 << EEV_B1) | (1 << EEV_B2);
}

void EEV_home(void)
{
	EEV_relative(700, EEV_CLOSE);
	
	position = 0;
}

void EEV_step(bool dir, bool hold_pulse)
{
	static uint8_t step = 0;
	
	if (hold_pulse) {}
	else if (dir)
	{
		position++;
		
		if (step == 7)
			step = 0;
		else
			step++;
	}
	else
	{
		if (position)
			position--;
		
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

void EEV_relative(uint16_t steps, bool dir)
{
	EEV_step(false, true);

	for (uint16_t i = 0; i < steps; i++)
		EEV_step(dir, false);

	EEV_step(false, true);
}

void EEV_absolute(uint16_t new_position)
{
	if (new_position <= EEV_min)
		new_position = 0;
	else if (new_position >= EEV_max)
		new_position = EEV_max;
	
	bool dir = EEV_CLOSE;
	if (new_position > position)
		dir = EEV_OPEN;
	
	EEV_step(false, true);
	
	EEV_relative(abs(position - new_position), dir);

	EEV_step(false, true);
}