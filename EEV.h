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

void EEV_handler(void);

void EEV_home(bool open_first);  // home EEV to fully closed position

void EEV_set_position(uint16_t pos);

bool EEV_mooving(void);  // true if EEV has not reached its command yet

uint16_t EEV_get_position(void);

#endif /* EEV_H_ */