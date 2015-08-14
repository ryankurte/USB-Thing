//DAC USB protocol to peripheral mapping

#include "services/dac_svc.h"

#include <stdint.h>

#include "em_usb.h"

#include "callbacks.h"
#include "protocol.h"
#include "peripherals/dac.h"

int dac_handle_setup(const USB_Setup_TypeDef *setup);
static int dac_config(const USB_Setup_TypeDef *setup);
static int dac_config_cb(const USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
static int dac_set(const USB_Setup_TypeDef *setup);
static int dac_set_cb(const USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);


EFM32_ALIGN(4)
extern uint8_t cmd_buffer[];

int dac_handle_setup(const USB_Setup_TypeDef *setup)
{
    switch (setup->wValue) {
    case USBTHING_CMD_DAC_CFG:
        return dac_config(setup);

    case USBTHING_CMD_DAC_SET:
        return dac_set(setup);
    }
    return USB_STATUS_REQ_UNHANDLED;
}

static int dac_config(const USB_Setup_TypeDef *setup)
{
    int res = USB_STATUS_REQ_ERR;

    CHECK_SETUP_OUT(USBTHING_CMD_DAC_CFG_SIZE);

    res = USBD_Read(0, cmd_buffer, USBTHING_CMD_DAC_CFG_SIZE, dac_config_cb);

    return res;
}

static int dac_config_cb(const USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
    (void)xferred;
    (void)remaining;

    struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

    DAC_configure();

    return USB_STATUS_OK;
}

static int dac_set(const USB_Setup_TypeDef *setup)
{
    int res = USB_STATUS_REQ_ERR;

    CHECK_SETUP_OUT(USBTHING_CMD_DAC_SET_SIZE);

    res = USBD_Read(0, cmd_buffer, USBTHING_CMD_DAC_SET_SIZE, dac_set_cb);

    return res;
}

static int dac_set_cb(const USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
    (void)xferred;
    (void)remaining;

    struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

    uint8_t enable = ctrl->dac_cmd.set.enable;
    uint16_t value = ctrl->dac_cmd.set.value;

    DAC_set(value);

    return USB_STATUS_OK;
}
