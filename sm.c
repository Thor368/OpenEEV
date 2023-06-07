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


enum sm_state_t { sms_init, sms_home, sms_start, sms_softstart, sms_decay, sms_regulate, sms_shutdown, sms_coastdown, sms_wait_call } sm_state;

uint64_t sm_timer, equal_timer, temp_timer;

uint16_t suc_temp, target_temp, suc_temp_pres;

void sm_init(void)
{
	sm_state = sms_init;
	suc_temp = 0;
	target_temp = 0;
}

void sm_superheat_regulator(void)
{
	
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
			
			equal_timer = ticks + 30000;
			sm_state = sms_home;
			break;
		
		case sms_home:
			EEV_home();
			
			sm_state = sms_start;
			break;
		
		case sms_start:
			EEV_absolute(500);
			relay_soft(RELAY_ON);
			
			sm_timer = ticks + 2000;
			sm_state = sms_softstart;
			break;
			
		case sms_softstart:
			if (ticks >= sm_timer)
			{
				relay_comp(RELAY_ON);
				
				sm_timer = ticks + 200;
				sm_state = sms_decay;
				}
			break;

		case sms_decay:		
			if (ticks >= sm_timer)
			{
				relay_comp(RELAY_DECAY);
				relay_soft(RELAY_OFF);
				relay_pumps(RELAY_ON);
				
				sm_timer = ticks + 150;
				temp_timer = ticks;
				sm_state = sms_regulate;
			}
			break;
		
		case sms_regulate:
			if (ticks >= temp_timer)
			{
				suc_temp = DS_read_temperature(temp_adr_suc_line);
//				target_temp = DS_read_temperature(temp_adr_target);
				DS_convert_T(DS_BROADCAST_ADR);
				
				suc_temp_pres = analog_get_suc_temp();
				
				if (target_temp > (TEMP_SETPOINT + TEMP_POS_HYST))
				{
					sm_state = sms_shutdown;
					break;
				}
				
				temp_timer += 1000;
			}
			
			if (ticks >= sm_timer)
			{
				sm_superheat_regulator();
				
				sm_timer += 150;
			}
			
			break;
		
		case sms_shutdown:
			relay_comp(RELAY_OFF);
			
			sm_timer = ticks + 60000;
			sm_state = sms_coastdown;
			break;
		
		case sms_coastdown:
			if (ticks >= sm_timer)
			{
				relay_pumps(RELAY_OFF);
				
				temp_timer = ticks;
				sm_state = sms_wait_call;
			}
			break;
			
		case sms_wait_call:
			if (ticks >= temp_timer)
			{
//				target_temp = DS_read_temperature(temp_adr_target);
				DS_convert_T(DS_BROADCAST_ADR);
				
				if (target_temp < (TEMP_SETPOINT - TEMP_NEG_HYST))
				{
					sm_state = sms_start;
					break;
				}

				temp_timer += 1000;
			}
			break;
	}
}