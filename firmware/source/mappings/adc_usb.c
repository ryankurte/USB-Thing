//ADC USB protocol to peripheral mapping

#include "mappings/adc_usb.h"

#include <stdint.h>

#include "em_usb.h"

#include "callbacks.h"
#include "protocol.h"
#include "peripherals/adc.h"

EFM32_ALIGN(4)
static uint8_t adc_value[USBTHING_CMD_ADC_GET_SIZE];

int adc_cb_configure(const USB_Setup_TypeDef *setup)
{
	CHECK_SETUP_OUT(USBTHING_CMD_ADC_CFG_SIZE);

	ADC_configure();

	return USB_STATUS_OK;
}

int adc_cb_set(const USB_Setup_TypeDef *setup)
{
	int8_t res;

	CHECK_SETUP_IN(USBTHING_CMD_ADC_GET_SIZE);

	ADC_get((uint32_t)adc_value);

	//TODO: receive 32bit value (pretty sure direction is wrong here?)
	res = USBD_Write(0, adc_value, USBTHING_CMD_ADC_SET_SIZE, NULL);

	return res;
}