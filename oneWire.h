/*
 * oneWire.h
 *
 * Created: 06.06.2023 08:39:05
 *  Author: Alexander Schroeder
 */ 


#ifndef ONEWIRE_H_
#define ONEWIRE_H_


#include <stdbool.h>
#include <stdint.h>

#define DS_CONV_TIME_12b	0b11
#define DS_CONV_TIME_11b	0b10
#define DS_CONV_TIME_10b	0b01
#define DS_CONV_TIME_9b		0b00

#define DS_BROADCAST_ADR	0

extern uint8_t temp_adr_suc_line[];

void DS_init(void);

bool DS_slave_present(void);  // true if there is a slave on the bus

bool DS_bus_busy(void);  // true if bus is busy

void DS_set_scratchpad(uint8_t* adr, uint8_t Rx, uint8_t TH, uint8_t TL);  // set conversion time, TH and TL

void DS_copy_scratchpad(uint8_t* adr);  // write scratchpad to EEPROM

uint8_t DS_read_scratchpad(uint8_t* adr, uint8_t* data);  // read whole scratchpad

int16_t DS_read_temperature(uint8_t* adr);  // read temperature in 0.1°C. returns -32768 on CRC error

void DS_convert_T(uint8_t* adr);  // start temperature conversion

uint8_t DS_read_ROM(uint8_t* adr);  // read single ROM returns 0 if CRC checks out


#endif /* ONEWIRE_H_ */