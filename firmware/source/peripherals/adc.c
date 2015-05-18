
#include "peripherals/adc.h"

#include <stdint.h>

#include "em_cmu.h"
#include "em_adc.h"

#include "platform.h"

void ADC_init()
{
    ADC_Init_TypeDef adc_init = ADC_INIT_DEFAULT;
    ADC_InitSingle_TypeDef single_init = ADC_INITSINGLE_DEFAULT;

    CMU_ClockEnable(ADC_CLOCK, true);

    //Todo: fine tuned config here

    ADC_Init(ADC_DEVICE, &adc_init);
    ADC_InitSingle(ADC_DEVICE, &single_init);
}

void ADC_close()
{
    ADC_Reset(ADC_DEVICE);

    CMU_ClockEnable(ADC_CLOCK, false);
}

uint32_t ADC_get(uint8_t channel)
{
    uint16_t res;

    ADC_Start(ADC_DEVICE, adcStartSingle);

    //Await ADC completion

    res = ADC_DataSingleGet(ADC_DEVICE);

    return res;
}