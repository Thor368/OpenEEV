/*
 * oneWire.c
 *
 * Created: 06.06.2023 08:39:14
 *  Author: main
 */ 

#include "config.h"
#include "oneWire.h"

#include <avr/io.h>
#include <util/delay.h>


#define DS_LOW			DS_DDR |= (1 << DS_IO)
#define DS_HI			DS_DDR &= ~(1 << DS_IO)
#define DS_IN			(DS_PIN & (1 << DS_IO))

#define DS_SEARCH_ROM	0xF0
#define DS_READ_ROM		0x33
#define DS_MATCH_ROM	0x55
#define DS_SKIP_ROM		0xCC
#define DS_ALARM_SEARCH	0xEC
#define DS_CONVERT_T	0x44
#define DS_WRITE_SCR	0x4E
#define DS_READ_SCR		0xBE
#define DS_COPY_SCR		0x48
#define DS_RECALL_EE	0xB8
#define DS_READ_POWER	0xB4


void DS_init(void)
{
	DS_PORT &= ~(1 << DS_IO);
	DS_HI;
}

bool DS_reset(void)
{
	DS_LOW;
	
	_delay_us(480);
	
	DS_HI;
	
	_delay_us(90);
	
	bool present = DS_IN;
	
	_delay_us(390);
	
	return present;
}

void DS_write_bit(bool value)
{
	DS_LOW;
		
	if (value)
	{
		_delay_us(1);

		DS_HI;
	
		_delay_us(60);
	}
	else
	{
		_delay_us(60);

		DS_HI;
	
		_delay_us(1);
	}
}

void DS_write_byte(uint8_t value)
{
	for (uint8_t i = 0; i < 8; i++)
		DS_write_bit(1 & (value >> i));
}

void DS_write_block(uint8_t* data, uint8_t len)
{
	while(len--)
		DS_write_byte(*data++);
}

void DS_ROM(uint64_t* adr)
{
	if (!adr)
		DS_write_byte(DS_SKIP_ROM);
	else
	{
		DS_write_byte(DS_MATCH_ROM);
		DS_write_block((uint8_t*) adr, 8);
	}
}

bool DS_read_bit(void)
{
	DS_LOW;
	
	_delay_us(1);
	
	DS_HI;
	
	_delay_us(5);
	
	bool read = DS_IN;
	
	_delay_us(55);
	
	return read;
}

uint8_t DS_read_byte(void)
{
	uint8_t result = 0;
	
	for (uint8_t i = 0; i < 8; i++)
		result |= DS_read_bit() << i;
	
	return result;
}

bool DS_slave_present(void)
{
	return !DS_reset();
}

bool DS_bus_busy(void)
{
	return !DS_read_bit();
}

void DS_set_scratchpad(uint64_t* adr, uint8_t Rx, uint8_t TH, uint8_t TL)
{
	DS_ROM(adr);
	
	DS_write_byte(DS_WRITE_SCR);
	
	DS_write_byte(TH);  // TH
	DS_write_byte(TL);  // TL
	DS_write_byte(0b11111 | (Rx << 5));  // CONFIG
}

void DS_copy_scratchpad(uint64_t* adr)
{
	DS_ROM(adr);
	
	DS_write_byte(DS_COPY_SCR);
}

uint16_t DS_read_temperature(uint64_t* adr)
{
	DS_ROM(adr);
	
	DS_write_byte(DS_READ_SCR);
	
	int32_t temp = DS_read_byte();
	temp |= DS_read_byte() << 8;
	
	DS_reset();
	
	temp = temp*625/10000;
	
	return temp;
}