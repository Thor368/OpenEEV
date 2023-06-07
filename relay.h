/*
 * relay.h
 *
 * Created: 06.06.2023 11:10:53
 *  Author: Alexander Schroeder
 */ 


#ifndef RELAY_H_
#define RELAY_H_


#include <stdbool.h>
#include <stdint.h>

#define RELAY_OFF		0
#define RELAY_ON		1
#define RELAY_DECAY		2

void relay_comp(uint8_t state);

void relay_soft(bool state);

void relay_pumps(bool state);


#endif /* RELAY_H_ */