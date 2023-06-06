/*
 * config.h
 *
 * Created: 06.06.2023 08:37:32
 *  Author: main
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

#define F_CPU 16000000

// EEV IO
#define EEV_PORT		PORTD
#define EEV_DDR			DDRD
#define EEV_A1			PD5
#define EEV_A2			PD3
#define EEV_B1			PD4
#define EEV_B2			PD2

// OneWire
#define DS_PORT			PORTB
#define DS_DDR			DDRB
#define DS_PIN			PINB
#define DS_IO			PB0

extern uint64_t ticks;  // ms tick counter


#endif /* CONFIG_H_ */