/*
 * nv.c
 *
 * Created: 14.06.2023 11:59:49
 *  Author: Alexander Schroeder
 */ 

#include "nv.h"
#include "config.h"

#include <avr/eeprom.h>

#define NV_VERSION			1

uint32_t sh_tc;
int16_t sh_setpoint;
int8_t sh_hysteresis;

uint8_t EE_VERSION EEMEM;
uint32_t EE_sh_tc EEMEM;
int16_t EE_sh_setpoint EEMEM;
int8_t EE_sh_hysteresis EEMEM;


void nv_update(void)
{
	eeprom_update_dword(&EE_sh_tc, sh_tc);
	eeprom_update_word((uint16_t*) &EE_sh_setpoint, sh_setpoint);
	eeprom_update_byte((uint8_t*) &EE_sh_hysteresis, sh_hysteresis);
}

void nv_read(void)
{
	sh_tc = eeprom_read_dword(&EE_sh_tc);
	sh_setpoint = eeprom_read_word((uint16_t*) &EE_sh_setpoint);
	sh_hysteresis = eeprom_read_byte((uint8_t*) &EE_sh_hysteresis);
}

void nv_reset(void)
{
	eeprom_write_byte(&EE_VERSION, NV_VERSION);

	sh_tc = sh_tc_default;
	sh_setpoint = sh_setpoint_default;
	sh_hysteresis = sh_hysteresis_default;
		
	nv_update();
}

void nv_init(void)
{
	if (eeprom_read_byte(&EE_VERSION) != NV_VERSION)
		nv_reset();
	else
		nv_read();
}