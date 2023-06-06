/*
 * EEV.h
 *
 * Created: 06.06.2023 08:32:15
 *  Author: main
 */ 


#include <stdbool.h>
#include <stdint.h>

#ifndef EEV_H_
#define EEV_H_


#define EEV_CLOSE		false
#define EEV_OPEN		true


void EEV_init(void);

void EEV_home(void);

void EEV_steps(uint16_t steps, bool dir);

void EEV_step(bool dir, bool hold_pulse);


#endif /* EEV_H_ */