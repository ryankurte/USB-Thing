
#include "handlers.h"

#include "em_usb.h"

#include "protocol.h"
#include "services/base_svc.h"
#include "services/gpio_svc.h"
#include "services/adc_svc.h"
#include "services/spi_svc.h"
#include "services/dac_svc.h"
#include "services/i2c_svc.h"

#include "peripherals/gpio.h"

// Buffer for setup messages
UBUF(cmd_buffer, 32);

// Handler for USB state change event
void state_change_cb(USBD_State_TypeDef old, USBD_State_TypeDef new)
{
    switch (new) {
    case USBD_STATE_CONFIGURED:
        spi_svc_start();
        i2c_svc_start();
        GPIO_conn_led_set(true);
        break;
    case USBD_STATE_NONE:
    case USBD_STATE_DEFAULT:
    case USBD_STATE_SUSPENDED:
    case USBD_STATE_ATTACHED:
    case USBD_STATE_POWERED:
    case USBD_STATE_ADDRESSED:
    case USBD_STATE_LASTMARKER:
        GPIO_conn_led_set(false);
        break;
    }
}

// Handler for USB setup commands
int setup_cmd_cb(const USB_Setup_TypeDef *setup)
{
    // bRequest argument used to route to module setup handlers
    switch (setup->bRequest) {
    case USBTHING_MODULE_BASE:
        return base_handle_setup(setup);

    case USBTHING_MODULE_GPIO:
        return gpio_handle_setup(setup);

    case USBTHING_MODULE_SPI:
        return spi_svc_handle_setup(setup);

    case USBTHING_MODULE_I2C:
        return i2c_svc_handle_setup(setup);

    case USBTHING_MODULE_ADC:
        return adc_handle_setup(setup);

    case USBTHING_MODULE_DAC:
        return dac_handle_setup(setup);
    }

    //Setup command was not handled
    return USB_STATUS_REQ_UNHANDLED;
}

