/*
 * oneWire.h
 *
 * Created: 06.06.2023 08:39:05
 *  Author: main
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

void DS_init(void);

bool DS_slave_present(void);  // true if there is a slave on the bus

bool DS_bus_busy(void);  // true if bus is busy

void DS_set_scratchpad(uint64_t* adr, uint8_t Rx, uint8_t TH, uint8_t TL);  // set conversion time, TH and TL

void DS_copy_scratchpad(uint64_t* adr);  // write scratchpad to EEPROM

uint16_t DS_read_temperature(uint64_t* adr);  // read temperature in 0.1°C

#endif /* ONEWIRE_H_ */