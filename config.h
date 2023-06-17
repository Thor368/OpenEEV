/*
 * config.h
 *
 * Created: 06.06.2023 08:37:32
 *  Author: Alexander Schroeder
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include <stdbool.h>

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

// Temperature sensor addresses
extern uint8_t temp_adr_suc_line[];
#define DS_SUC_ADR		0x28, 0xe6, 0x14, 0x77, 0x91, 0x0a, 0x02, 0x10
//#define DS_TARGET_ADR	0x28, 

// Superheat controller
// reset values
#define sh_tc_default			120000    // ms intervall in which the controller is executed
#define sh_setpoint_default		0     // 0.1°C superheat setpoint
#define sh_hysteresis_default	20      // 0.1K superheat hysteresis

// working variables
extern uint32_t sh_tc;
extern int16_t sh_setpoint;
extern int8_t sh_hysteresis;

extern uint64_t ticks;  // ms tick counter
extern bool call;  // call for cooling/heating


#endif /* CONFIG_H_ */