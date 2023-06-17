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
#include <stdio.h>

int16_t EEV_position_actual, EEV_position_command;
uint8_t step = 0;

uint64_t eev_timer = 0;

enum EEV_states { eev_idle, eev_start, eev_running, eev_stop} EEV_state;

void EEV_init(void)
{
	EEV_PORT &= ~((1 << EEV_A1) | (1 << EEV_A2) | (1 << EEV_B1) | (1 << EEV_B2));
	EEV_DDR |= (1 << EEV_A1) | (1 << EEV_A2) | (1 << EEV_B1) | (1 << EEV_B2);
	
	EEV_state = eev_idle;
	EEV_position_command = 0;
	EEV_position_actual = 0;
}

void EEV_update(void)
{
	uint8_t p = EEV_PORT & ~((1 << EEV_A1) | (1 << EEV_A2) | (1 << EEV_B1) | (1 << EEV_B2));
	
	switch (step)
	{
		case 0:
			p |= (1 << EEV_A1);
			break;

		case 1:
			p |= (1 << EEV_A1) | (1 << EEV_B1);
			break;

		case 2:
			p |= (1 << EEV_B1);
			break;

		case 3:
			p |= (1 << EEV_A2) | (1 << EEV_B1);
			break;

		case 4:
			p |= (1 << EEV_A2);
			break;

		case 5:
			p |= (1 << EEV_A2) | (1 << EEV_B2);
			break;

		case 6:
			p |= (1 << EEV_B2);
			break;

		case 7:
			p |= (1 << EEV_A1) | (1 << EEV_B2);
			break;
	}
	
	EEV_PORT = p;
}

void EEV_release(void)
{
	EEV_PORT &= ~((1 << EEV_A1) | (1 << EEV_A2) | (1 << EEV_B1) | (1 << EEV_B2));
}

void EEV_step(bool dir)
{
	if (dir)
	{
		if (step < 7)
			step++;
		else
			step = 0;
	}
	else
	{
		if (step)
			step--;
		else
			step = 7;
	}
	
	EEV_update();
}

void EEV_home(bool open_first)
{
	if (open_first)
	{
		for (uint16_t i = 0; i < 500; i++)
		{
			EEV_step(true);
			_delay_ms(12);
		}
	}

	for (uint16_t i = 0; i < 700; i++)
	{
		EEV_step(false);
		_delay_ms(12);
	}
	
	EEV_position_actual = 0;
}

void EEV_handler(void)
{
	switch(EEV_state)
	{
		case eev_idle:
			if (EEV_position_actual != EEV_position_command)
			{
// 				printf("cmd != pos\n");
// 				printf("start step... ");
				EEV_update();
				
				eev_timer = ticks + 500;
				EEV_state = eev_start;
			}
			break;
		
		case eev_start:
			if (ticks >= eev_timer)
			{
//				printf("done\n");
				eev_timer = ticks + 12;
				EEV_state = eev_running;
			}
			break;
		
		case eev_running:
			if (ticks >= eev_timer)
			{
				if (EEV_position_command > EEV_position_actual)
				{
					EEV_step(true);
					EEV_position_actual++;
				}
				else if (EEV_position_command < EEV_position_actual)
				{
					EEV_step(false);
					EEV_position_actual--;
				}
				else
				{
//					printf("stop step... ");
					eev_timer = ticks + 500;
					EEV_state = eev_stop;
					break;
				}
				
				eev_timer += 12;
			}
			break;
		
		case eev_stop:
			if (ticks >= eev_timer)
			{
				EEV_release();
//				printf("done\n");
				EEV_state = eev_idle;
			}
	}
}

void EEV_set_position(uint16_t pos)
{
	if (pos > EEV_max)
		EEV_position_command = EEV_max;
	else
		EEV_position_command = pos;
	
//	printf("new: %d, cor: %d", pos, EEV_position_command);
}

uint16_t EEV_get_real_position(void)
{
	return EEV_position_actual;
}

uint16_t EEV_get_command_position(void)
{
	return EEV_position_command;
}
