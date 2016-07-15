
#include "services/gpio_svc.h"

#include <stdint.h>

#include "em_usb.h"

#include "handlers.h"
#include "protocol.h"
#include "peripherals/gpio.h"

static int gpio_config(const USB_Setup_TypeDef *setup);
static int gpio_config_cb(const USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
static int gpio_set(const USB_Setup_TypeDef *setup);
static int gpio_set_cb(const USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
static int gpio_get(const USB_Setup_TypeDef *setup);

extern uint8_t cmd_buffer[];

int gpio_handle_setup(const USB_Setup_TypeDef *setup)
{
	switch (setup->wValue) {
	case USBTHING_GPIO_CMD_CONFIG:
		return gpio_config(setup);

	case USBTHING_GPIO_CMD_SET:
		return gpio_set(setup);

	case USBTHING_GPIO_CMD_GET:
		return gpio_get(setup);
	}
	return USB_STATUS_REQ_UNHANDLED;
}

static int gpio_config(const USB_Setup_TypeDef *setup)
{
    int res = USB_STATUS_REQ_ERR;

    CHECK_SETUP_OUT(USBTHING_CMD_GPIO_CFG_SIZE);

    res = USBD_Read(0, cmd_buffer, USBTHING_CMD_GPIO_CFG_SIZE, gpio_config_cb);

    return res;
}

static int gpio_config_cb(const USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
    (void)xferred;
    (void)remaining;

    struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

    uint8_t pin = ctrl->gpio_cmd.config.pin;
	bool output = (ctrl->gpio_cmd.config.mode == USBTHING_GPIO_MODE_INPUT) ? false : true;
	bool pull_enabled = (ctrl->gpio_cmd.config.pull != 0) ? true : false;
	bool pull_direction = (ctrl->gpio_cmd.config.pull != 0) ? true : false;

    GPIO_configure(pin, output, pull_enabled, pull_direction);

    return USB_STATUS_OK;
}

static int gpio_set(const USB_Setup_TypeDef *setup)
{
    int res = USB_STATUS_REQ_ERR;

    CHECK_SETUP_OUT(USBTHING_CMD_GPIO_SET_SIZE);

    res = USBD_Read(0, cmd_buffer, USBTHING_CMD_GPIO_SET_SIZE, gpio_set_cb);

    return res;
}

static int gpio_set_cb(const USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
    (void)xferred;
    (void)remaining;

    struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

    uint8_t pin = ctrl->gpio_cmd.set.pin;
	bool output = (ctrl->gpio_cmd.set.level == USBTHING_GPIO_LEVEL_LOW) ? false : true;

    GPIO_set(pin, output);

    return USB_STATUS_OK;
}

static int gpio_get(const USB_Setup_TypeDef *setup)
{
	CHECK_SETUP_IN(USBTHING_CMD_GPIO_GET_SIZE);

	uint8_t pin = setup->wIndex;

	struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

	ctrl->gpio_cmd.get.level = GPIO_get(pin);

	//TODO: respond
	return USBD_Write(0, cmd_buffer, USBTHING_CMD_GPIO_GET_SIZE, NULL);
}



