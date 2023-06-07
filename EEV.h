/*
 * EEV.h
 *
 * Created: 06.06.2023 08:32:15
 *  Author: Alexander Schroeder
 */ 


#include <stdbool.h>
#include <stdint.h>

#ifndef EEV_H_
#define EEV_H_


#define EEV_CLOSE		false
#define EEV_OPEN		true

#define EEV_min			50
#define EEV_max			500

void EEV_init(void);

void EEV_home(void);  // home EEV to fully closed position

void EEV_step(bool dir, bool hold_pulse);  // single step EEV

void EEV_relative(uint16_t steps, bool dir);  // relative movement

void EEV_absolute(uint16_t new_position);  // set EEV to absolute position (0-100%)


#endif /* EEV_H_ */