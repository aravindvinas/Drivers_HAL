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
 
#ifndef __HX_711_H
#define __HX_711_H

#include "stm32f3xx_hal.h"

//Channel No and Config
#define CHANNEL_A_64     0x03
#define CHANNEL_A_128    0x01
#define CHANNEL_B_32     0x02

//Parameter struct
typedef struct{
    GPIO_Typedef *dout;
    GPIO_Typedef *sck;
    uint16_t dout_pin;
    uint16_t sck_pin;
    uint8_t channel_gain;
}hx711_Typedef;

uint32_t load_val_read(hx711_Typdef *hx711);

#endif
