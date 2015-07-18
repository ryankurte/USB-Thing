
#include "peripherals/adc.h"

#include <stdint.h>

#include "em_cmu.h"
#include "em_adc.h"
#include "em_gpio.h"

#include "platform.h"

static int voltage_reference = adcRefVDD;

void ADC_init(uint32_t reference)
{
    ADC_Init_TypeDef adc_init = ADC_INIT_DEFAULT;

    adc_init.timebase = ADC_TimebaseCalc(0);
    adc_init.prescale = ADC_PrescaleCalc(7000000, 0);

    //Todo: fine tuned config here
    voltage_reference = reference;

    CMU_ClockEnable(ADC_CLOCK, true);

    ADC_Init(ADC_DEVICE, &adc_init);
}

void ADC_close()
{
    ADC_Reset(ADC_DEVICE);

    CMU_ClockEnable(ADC_CLOCK, false);
}

uint32_t ADC_get(uint8_t channel)
{
    uint16_t res;
    ADC_InitSingle_TypeDef single_init = ADC_INITSINGLE_DEFAULT;

    single_init.input = channel;
    single_init.reference = voltage_reference;
    single_init.acqTime = adcAcqTime32;

    ADC_InitSingle(ADC_DEVICE, &single_init);

    ADC_Start(ADC_DEVICE, adcStartSingle);

    //Await ADC completion
    while (ADC0->STATUS & ADC_STATUS_SINGLEACT) ;

    res = ADC_DataSingleGet(ADC_DEVICE);

    return res;
}