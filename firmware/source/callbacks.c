/**************************************************************************//**
 * THIS FILE HAS BEEN MODIFIED FROM THE ORIGINAL
 *
 * @file callbacks.c
 * @brief USB callback functions
 * @author Energy Micro AS
 * @version 1.01
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/
#include <string.h>
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "em_gpio.h"
#include "em_usart.h"

#include "callbacks.h"
#include "protocol.h"
#include "platform.h"

#include "services/base_svc.h"
#include "services/gpio_svc.h"
#include "services/adc_svc.h"
#include "services/spi_svc.h"
#include "services/dac_svc.h"
#include "services/i2c_svc.h"

#include "peripherals/gpio.h"
#include "peripherals/i2c.h"
#include "peripherals/spi.h"
#include "peripherals/dac.h"


#define BUFFERSIZE 500

/* Buffer to receive incoming messages. Needs to be
 * WORD aligned and an integer number of WORDs large */

UBUF(cmd_buffer, 32);


/* Counter to increase when receiving a 'tick' message */
int tickCounter = 0;

extern uint8_t button0message[];
extern uint8_t button1message[];


/**********************************************************
 * Called by the USB stack when a state change happens.
 *
 * @param oldState
 *   The previous state
 *
 * @param newState
 *   The new (current) state
 *
 **********************************************************/
void stateChange(USBD_State_TypeDef oldState, USBD_State_TypeDef newState)
{
    /* Print state transition to debug output */
    printf("\n%s => %s", USBD_GetUsbStateName(oldState), USBD_GetUsbStateName(newState));

    if ((newState == USBD_STATE_NONE)
        || (newState == USBD_STATE_DEFAULT)
        || (newState == USBD_STATE_SUSPENDED)) {
        //Initial states, not configured
        GPIO_conn_led_set(false);

    } else if (newState == USBD_STATE_CONFIGURED) {

        // Start services with dedicated endpoints
        spi_svc_start();
        i2c_svc_start();

        GPIO_conn_led_set(true);

    } else if ( newState != USBD_STATE_SUSPENDED ) {
        //Todo: flash
    }
}

int setupCmd(const USB_Setup_TypeDef *setup)
{
    //TODO: handle commands

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

    //Signal command was not handled
    return USB_STATUS_REQ_UNHANDLED;
}
