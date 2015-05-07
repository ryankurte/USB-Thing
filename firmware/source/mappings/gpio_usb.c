
#include "mappings/gpio_usb.h"

#include <stdint.h>

#include "em_usb.h"

#include "callbacks.h"
#include "protocol.h"
#include "peripherals/gpio.h"

EFM32_ALIGN(4)
static uint8_t pin_value[USBTHING_CMD_GPIO_GET_SIZE];

int gpio_cb_configure(const USB_Setup_TypeDef *setup)
{
	CHECK_SETUP_OUT(USBTHING_CMD_GPIO_CFG_SIZE);

	uint8_t pin = setup->wIndex;
	bool output = ((setup->wValue & USBTHING_GPIO_CFG_MODE_OUTPUT) != 0) ? true : false;
	bool pull_enabled = ((setup->wValue & USBTHING_GPIO_CFG_PULL_ENABLE) != 0) ? true : false;
	bool pull_direction = ((setup->wValue & USBTHING_GPIO_CFG_PULL_HIGH) != 0) ? true : false;

	GPIO_configure(pin, output, pull_enabled, pull_direction);

	return USB_STATUS_OK;
}

int gpio_cb_set(const USB_Setup_TypeDef *setup)
{
	CHECK_SETUP_OUT(USBTHING_CMD_GPIO_SET_SIZE);

	GPIO_set(setup->wIndex, setup->wValue);

	return USB_STATUS_OK;
}

int gpio_cb_get(const USB_Setup_TypeDef *setup)
{
	int res = USB_STATUS_REQ_ERR;

	CHECK_SETUP_OUT(USBTHING_CMD_GPIO_GET_SIZE);

	uint8_t pin = setup->wIndex;
	pin_value[0] = GPIO_get(pin);

	//TODO: respond
	res = USBD_Write(0, pin_value, USBTHING_CMD_FIRMWARE_GET_SIZE, NULL);

	return res;
}