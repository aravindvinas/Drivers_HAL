#include "hx711.h"

static uint8_t first_flag = 0;
static uin32_t data_set[16];

static float mov_win_avg(uint32_t new_val)
{
    //moving window average
    for(uint8_t i=1; i<15; i++)
    {
        data_set[i-1] = data_set[i];
    }

    data_set[15] = new_val;

    for(uint8_t i=0; i<16; i++)
    {
        new_val = new_val + data_set[i];
    }

    new_val = new_val/16;
    new_val = new_val - hx.offset;
    new_val = (float)new_val/(hx.calibration);

    return new_val;
}

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

static uint32_t probe_weight(hx711_typedef hx)
{
    uint32_t load_val = 0;

    while (HAL_GPIO_ReadPin(hx.dout, hx.pin_dout)==1);

    for (uint8_t i = 0; i < 24; i++)
    {
    	HAL_GPIO_WritePin(hx.sck, hx.pin_sck, GPIO_PIN_SET);

        if (HAL_GPIO_ReadPin(hx.dout, hx.pin_dout))
        {
        	load_val = load_val << 1;
        	load_val |= 0x00000001;
        }

        else
        {
        	load_val = load_val << 1;
        	load_val &= 0xFFFFFFFE;
        }

    	HAL_GPIO_WritePin(hx.sck, hx.pin_sck, GPIO_PIN_RESET);
    }

    for (uint8_t i=0; i<hx.gain; i++)
    {
    	HAL_GPIO_WritePin(hx.sck, hx.pin_sck, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(hx.sck, hx.pin_sck, GPIO_PIN_RESET);
    }

    load_val = load_val ^ 0x800000;

    return load_val;
}

float get_weight(hx711_typedef hx)
{
    uint32_t probe = 0;
    float val;
    if(!flag)
    {
        flag = 1;

        //populate data_set first
        for(uint8_t i=0; i<16; i++)
        {
            data_set[i] = probe_weight(hx);
        }

        probe = probe_weight(hx);
        val = mov_win_avg(probe);
    }

    else
    {
        probe = probe_weight(hx);
        val = mov_win_avg(probe);
    }

    return val;
}

hx711_typedef tare_calc(hx711_typedef hx)
{
	uint32_t tare = 0;
	uint8_t i = 0;

	while(i<16){
		tare = tare + probe_weight(hx);
		i++;
	}

	tare = tare/16;
	hx.offset = tare;

    return hx;
}

hx711_typedef calibrate(hx711_typedef hx, float actual_mass)
{
	//get weight before calibration is set
	float raw = get_weight(hx);

	//new calibration value
	float new_cal = 0;

	//calculating new calibration
	new_cal = (hx.calibration)*raw;	
	new_cal = new_cal/actual_mass;	

	hx.calibration = new_cal;
        
    return hx;
}
