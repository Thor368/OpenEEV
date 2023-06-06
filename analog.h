/*
 * analog.h
 *
 * Created: 06.06.2023 18:00:50
 *  Author: Alexander Schroeder
 */ 


#ifndef ANALOG_H_
#define ANALOG_H_

extern uint16_t p;

void analog_init(void);

uint16_t analog_read(uint8_t channel);


#endif /* ANALOG_H_ */