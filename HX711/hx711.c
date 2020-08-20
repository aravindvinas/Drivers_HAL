#include "hx711.h"


hx711_typedef hx711_init(GPIO_TypeDef *port_dout, GPIO_TypeDef *port_sck, uint16_t dout, uint16_t sck, uint8_t channel_gain)
{
	hx711_typedef config;

	config.dout = port_dout;
	config.sck = port_sck;
	config.pin_dout = dout;
	config.pin_sck = sck;
	config.offset = 0;
	config.calibration = 1;
	config.gain = channel_gain;

	return config;
}

float get_weight(hx711_typedef *hx)
{
    int load_val = 0;

    while (HAL_GPIO_ReadPin(hx->dout, hx->pin_dout)==1);

    for (uint8_t i = 0; i < 24; i++)
    {
    	HAL_GPIO_WritePin(hx->sck, hx->pin_sck, GPIO_PIN_SET);

        if (HAL_GPIO_ReadPin(hx->dout, hx->pin_dout))
        {
        	load_val = load_val << 1;
        	load_val |= 0x00000001;
        }

        else
        {
        	load_val = load_val << 1;
        	load_val &= 0xFFFFFFFE;
        }

    	HAL_GPIO_WritePin(hx->sck, hx->pin_sck, GPIO_PIN_RESET);
    }

    for (uint8_t i=0; i<hx->gain; i++)
    {
    	HAL_GPIO_WritePin(hx->sck, hx->pin_sck, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(hx->sck, hx->pin_sck, GPIO_PIN_RESET);
    }

    load_val = load_val ^ 0x800000;

    load_val = float(load_val);
    load_val = load_val - hx->offset;
    load_val = load_val/hx->calibration;

    return load_val;
}

void tare_calc(hx711_typedef *hx)
{
	float tare = 0;
	uint8_t i = 0;

	while(i<16){
		tare = tare + get_weight(hx);
		i++;
	}

	tare = tare/16;
	hx->offset = tare;
}

void calibrate(hx711_typedef *hx, uint32_t actual_mass)
{
	//get weight before calibration is set
	float raw = float(get_weight);

	//new calibration value
	float new_cal = 0;

	//calculating new calibration
	new_cal = old_cal*raw;	
	new_cal = new_cal/actual_mass;	

	hx->calibration = new_cal;
}
