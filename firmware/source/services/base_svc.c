/**
 * Base service
 * Provides base control and query operations
 * 
 * @author Ryan Kurte
 * @date Fri Jul 17 18:40:08 2015
 */

#include "services/base_svc.h"

#include <stdint.h>

#include "em_usb.h"
#include "em_device.h"

#include "callbacks.h"
#include "protocol.h"
#include "version.h"

STATIC_UBUF(cmd_buffer, 32);

EFM32_ALIGN(4)
uint8_t firmware_version[] = SOFTWARE_VERSION;

static int led_set_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
static int firmware_get(const USB_Setup_TypeDef *setup);
static int led_set(const USB_Setup_TypeDef *setup);

int base_handle_setup(const USB_Setup_TypeDef *setup)
{
    switch (setup->wValue) {
    case BASE_CMD_NOOP:
        __asm("nop");
        break;
    case BASE_CMD_SERIAL_GET:
        //serial_get(setup);
        break;
    case BASE_CMD_FIRMWARE_GET:
        firmware_get(setup);
        break;
    case BASE_CMD_LED_SET:
        led_set(setup);
        break;
    case BASE_CMD_RESET:
        NVIC_SystemReset();
        break;
    }
}

static int firmware_get(const USB_Setup_TypeDef *setup)
{
    int res = USB_STATUS_REQ_ERR;

    CHECK_SETUP_IN(USBTHING_FIRMWARE_MAX_SIZE);

    res = USBD_Write(0, firmware_version, sizeof(firmware_version), NULL);

    return res;
}

static int led_set(const USB_Setup_TypeDef *setup)
{
    int res = USB_STATUS_REQ_ERR;

    CHECK_SETUP_OUT(USBTHING_CMD_LED_SET_SIZE);

    res = USBD_Read(0, cmd_buffer, USBTHING_CMD_LED_SET_SIZE, led_set_cb);

    return res;
}

static int led_set_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
    (void)xferred;
    (void)remaining;

    struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

    GPIO_led_set(ctrl->base_cmd.led_set.pin, ctrl->base_cmd.led_set.enable);

    return USB_STATUS_OK;
}