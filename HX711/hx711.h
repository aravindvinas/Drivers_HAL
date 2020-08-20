/*
 * A simple driver to read data from HX711 amplifier IC.
 * 
 * This Driver uses ST's Hardware Abstraction Layer 
 * 
 * Enable 2 pins for SCK and DOUT lines
 * 
 * DOUT : Input, PullUp, Low Freq
 * SCK : Output, NoPull, Low Freq
 * 
 */
 
#ifndef __HX711_H
#define	__HX711_H

#include "stm32f1xx_hal.h"

// Channel and gain configuration
#define CHANNEL_A_128	0x01
#define CHANNEL_A_64	0x03
#define CHANNEL_B_32	0x02

// HX711 configuration struct
typedef struct 
{
	GPIO_TypeDef *dout;
	GPIO_TypeDef *sck;
	uint16_t pin_dout;
	uint16_t pin_sck;
	float offset;
	float calibration;
	uint8_t gain;
} hx711_typedef;

hx711_typedef hx711_init(GPIO_TypeDef *port_dout, GPIO_TypeDef *port_sck, uint16_t dout, uint16_t sck, uint8_t channel_gain);
float get_weight(hx711_typedef *hx);
void tare_calc(hx711_typedef *hx);
void calibrate(hx711_typedef *hx, uint32_t actual_mass);


#endif
