
#include "adc.h"

#include <stdint.h>

#include "em_cmu.h"
#include "em_adc.h"

int8_t ADC_init() 
{
    ADC_Init_TypeDef adc_init = ADC_INIT_DEFAULT;
    ADC_InitSingle_TypeDef single_init = ADC_INITSINGLE_DEFAULT

    CMU_ClockEnable(cmuClockADC0, true);

    //Todo: fine tuned config here

    ADC_Init(ADC_DEV, &adc_init);
    ADC_InitSingle(ADC_DEV, &single_init);
}

int8_t ADC_close()
{
    ADC_Reset(ADC_DEV);

    CMU_ClockEnable(cmuClockADC0, false);
}

uint16_t ADC_read(uint8_t channel) 
{
    uint16_t res;

    ADC_Start(ADC_DEV, adcStartSingle);

    //Await ADC completion

    res = ADC_DataSingleGet(ADC_DEV);

    return res;
}