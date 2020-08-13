#include "hx711.h"

uint32_t load_val_read(hx711_Typedef *hx711)
{
    uint32_t load_val = 0;

    //Wait until ready state
    while(HAL_GPIO_ReadPin(hx711->dout, hx711->dout_pin) == 1);

    for(int i=0; i<24; i++)
    {
        HAL_GPIO_WritePin(hx711->sck, hx711->sck.pin, GPIO_PIN_SET);

        //if DOUT reads 1
        if(HAL_GPIO_ReadPin(hx711->dout, hx711->dout.pin) == 1){
            load_val = load_val << 1;
            load_val |= 0x00000001; 
        }

        //if DOUT reads 0
        else{
            load_val = load_val << 1;
            load_val &= 0xFFFFFFFE; 
        }

        HAL_GPIO_WritePin(hx711->sck, hx711->sck.pin, GPIO_PIN_RESET);
    }

    //Setting gain value for next conv
    for(int i=0; i<hx711->channel_gain; i++)
    {
        HAL_GPIO_WritePin(hx711->sck, hx711->sck.pin, GPIO_PIN_SET);
        HAL_Delay(1);
        HAL_GPIO_WritePin(hx711->sck, hx711->sck.pin, GPIO_PIN_RESET);
    }

    //Bitwise XOR to get absolute value
    load_val = load_val ^ 0x800000;

    //return measured load value
    return load_val;
}


