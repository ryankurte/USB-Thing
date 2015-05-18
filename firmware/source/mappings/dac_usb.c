//DAC USB protocol to peripheral mapping

#include "mappings/dac_usb.h"

#include <stdint.h>

#include "em_usb.h"

#include "callbacks.h"
#include "protocol.h"
#include "peripherals/dac.h"

EFM32_ALIGN(4)
static uint8_t dac_value[USBTHING_CMD_DAC_SET_SIZE];

int dac_cb_configure(const USB_Setup_TypeDef *setup)
{
	CHECK_SETUP_OUT(USBTHING_CMD_DAC_CFG_SIZE);

	DAC_configure();

	return USB_STATUS_OK;
}

int dac_cb_enable(const USB_Setup_TypeDef *setup)
{
	CHECK_SETUP_OUT(USBTHING_CMD_DAC_ENABLE_SIZE);

	DAC_enable(setup->wValue);

	return USB_STATUS_OK;
}

int dac_cb_set(const USB_Setup_TypeDef *setup)
{
	int8_t res;

	CHECK_SETUP_IN(USBTHING_CMD_DAC_SET_SIZE);

	//TODO: receive 32bit value (pretty sure direction is wrong here?)
	res = USBD_Read(0, dac_value, USBTHING_CMD_DAC_SET_SIZE, NULL);

	//TODO: cast between types? (perhaps HTON function would work here)
	DAC_set((uint32_t)dac_value);

	return USB_STATUS_OK;
}