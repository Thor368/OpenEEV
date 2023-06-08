/*
 * analog.h
 *
 * Created: 06.06.2023 18:00:50
 *  Author: Alexander Schroeder
 */ 


#ifndef ANALOG_H_
#define ANALOG_H_


#include <stdint.h>

extern uint16_t p;

void analog_init(void);

int16_t analog_get_suc_pressure(void);  // returns pressure in mbar

int16_t analog_get_suc_temp(void);  // returns calculated suction temperature from pressure in 0.1C


#endif /* ANALOG_H_ */