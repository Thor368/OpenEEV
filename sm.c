/*
 * sm.c
 *
 * Created: 07.06.2023 11:24:20
 *  Author: Alexander Schroeder
 */ 

#include "config.h"
#include "relay.h"
#include "EEV.h"
#include "oneWire.h"
#include "analog.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define sign(x)				((x > 0) - (x < 0))

enum sm_states { sms_init, sms_home, sms_start, sms_softstart, sms_decay, sms_regulate, sms_coastdown, sms_wait_call } sm_state;

uint8_t temp_adr_suc_line[] = {DS_SUC_ADR};

uint64_t sm_timer, temp_timer;

int16_t suc_temp, suc_temp_pres, superheat;

bool call = true;

void sm_init(void)
{
	sm_state = sms_init;
	suc_temp = 0;
	suc_temp_pres = 0;
	superheat = 0;
}

void sm_handler(void)
{
	switch (sm_state)
	{
		case sms_init:
			sm_init();
			
			relay_comp(RELAY_OFF);
			relay_soft(RELAY_OFF);
			relay_pumps(RELAY_OFF);
			
			sm_state = sms_home;
			
			printf("sm start\n");
			break;
		
		case sms_home:
			printf("homing eev... ");
			EEV_home(true);
			printf("done.\n");
			
			sm_state = sms_start;
			break;
		
		case sms_start:
			printf("call\n");
			printf("set eev to min... ");
			EEV_set_position(60);
			printf("done\n");
			printf("compressor start: soft... ");
			relay_soft(RELAY_ON);
			
			sm_timer = ticks;
			sm_state = sms_softstart;
			break;
			
		case sms_softstart:
			if (ticks >= (sm_timer + 1000))
			{
				printf("hard... ");
				relay_comp(RELAY_ON);
				
				sm_timer = ticks + 200;
				sm_state = sms_decay;
			}
			break;

		case sms_decay:		
			if (ticks >= sm_timer)
			{
				printf("done.\n");
				relay_comp(RELAY_DECAY);
				relay_soft(RELAY_OFF);
				printf("pumps on\n");
				relay_pumps(RELAY_ON);
				
				sm_timer = ticks + sm_timer;
				temp_timer = ticks;
				sm_state = sms_regulate;
				printf("startup complete\n");
			}
			break;
		
		case sms_regulate:
			if (ticks >= temp_timer)
			{
				suc_temp = DS_read_temperature(temp_adr_suc_line);
				DS_convert_T(DS_BROADCAST_ADR);
				
				suc_temp_pres = analog_get_suc_temp();
				superheat = suc_temp - suc_temp_pres;
				
				printf("suck_t: %+04d, suck_pt: %+04d, superheat: %+04d, eev: %03d\n", suc_temp, suc_temp_pres, superheat, EEV_get_real_position());
				
				if (!call)
				{
					printf("no call: coast down\n");
					relay_comp(RELAY_OFF);
			
					sm_timer = ticks + 60000;
					sm_state = sms_coastdown;
					break;
				}
				
				temp_timer += 1000;
			}
			
			int16_t error = superheat - sh_setpoint;
			if ((ticks >= sm_timer) && (abs(error) > sh_hysteresis))
			{
				int16_t pos = EEV_get_command_position();
				
				if (abs(error) <= 4)
					error = sign(error);
				else
					error = error >> 2;
				
				pos += error;
// 				if (pos && (error < sh_hysteresis))
// 					pos--;
// 				else if (superheat > sh_hysteresis)
// 					pos++;

				if (pos < 0)
					pos = 0;
				else if (pos > EEV_max)
					pos = EEV_max;
				EEV_set_position(pos);
				
				sm_timer = ticks + sh_tc;
			}
			
			break;
		
		case sms_coastdown:
			if (ticks >= sm_timer)
			{
				printf("shutdown complete: standby\n");
				relay_pumps(RELAY_OFF);
				
				temp_timer = ticks;
				sm_state = sms_wait_call;
			}
			break;
			
		case sms_wait_call:
			if (call)
				sm_state = sms_start;
			
			break;
	}
}