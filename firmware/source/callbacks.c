/**************************************************************************//**
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
#include "services/spi_svc.h"
#include "services/dac_svc.h"

#include "peripherals/gpio.h"
#include "peripherals/i2c.h"
#include "peripherals/spi.h"
#include "peripherals/dac.h"


#define BUFFERSIZE 500

/* Buffer to receive incoming messages. Needs to be
 * WORD aligned and an integer number of WORDs large */

STATIC_UBUF(cmd_buffer, 32);
STATIC_UBUF(i2c_receive_buffer, BUFFERSIZE);
STATIC_UBUF(i2c_transmit_buffer, BUFFERSIZE);


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
        /* Start waiting for the 'tick' messages */
        spi_cb_start();
        USBD_Read(EP2_OUT, i2c_receive_buffer, BUFFERSIZE, i2c_data_receive_callback);
        GPIO_conn_led_set(true);

    } else if ( newState != USBD_STATE_SUSPENDED ) {
        //Todo: flash
    }
}

static int i2c_configured = 0;

int i2c_configure(const USB_Setup_TypeDef *setup)
{
    int res = USB_STATUS_REQ_ERR;

    CHECK_SETUP_OUT(USBTHING_I2C_CFG_SIZE);

    uint8_t mode = setup->wValue;
    uint8_t flags = setup->wIndex;
    uint32_t baud;

    //Set baud rate based on mode
    switch (mode) {
    case USBTHING_I2C_SPEED_STANDARD:
        baud = 100000;
        break;
    case USBTHING_I2C_SPEED_FULL:
        baud = 400000;
        break;
    case USBTHING_I2C_SPEED_FAST:
        baud = 1000000;
        break;
    case USBTHING_I2C_SPEED_HIGH:
        baud = 3200000;
        break;
    }

    //Initialize I2C
    I2C_init(baud);

    i2c_configured = 1;

    return USB_STATUS_OK;
}

int setupCmd(const USB_Setup_TypeDef *setup)
{
    //TODO: handle commands

    switch (setup->bRequest) {
    case USBTHING_MODULE_BASE:
        return base_handle_service(setup);

    case USBTHING_MODULE_GPIO:
        return gpio_handle_service(setup);

    case USBTHING_CMD_I2C_CFG:
        return i2c_configure(setup);

    case USBTHING_CMD_SPI_CFG:
        return spi_cb_configure(setup);

    case USBTHING_CMD_DAC_CFG:
        return dac_cb_configure(setup);

    case USBTHING_CMD_DAC_EN:
        return dac_cb_enable(setup);

    case USBTHING_CMD_DAC_SET:
        return dac_cb_set(setup);
    }

    //Signal command was not handled
    return USB_STATUS_REQ_UNHANDLED;
}

int i2c_data_sent_callback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
    /* Remove warnings for unused variables */
    (void)xferred;
    (void)remaining;

    //Restart EP_OUT
    USBD_Read(EP1_OUT, i2c_receive_buffer, BUFFERSIZE, i2c_data_receive_callback);

    if ( status != USB_STATUS_OK ) {
        /* Handle error */
    }
    return USB_STATUS_OK;
}

int i2c_data_receive_callback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
    //Remove unused variable warnings
    (void)xferred;
    (void)remaining;

    //TODO: what if i2c is not initialized?

    if ( status == USB_STATUS_OK ) {
        struct usbthing_i2c_transfer_s *config = (struct usbthing_i2c_transfer_s *) i2c_receive_buffer;

        //TODO: bounds checking of num_write, num_read vs. xferred and maximum

        //Call hardware function based on mode
        switch (config->mode) {
        case USBTHING_I2C_MODE_WRITE:
            I2C_write(config->address, config->num_write, i2c_receive_buffer + sizeof(usbthing_i2c_transfer_s));
            //nb. dummy write back to USB (allows ordering/exclusion)
            USBD_Write(EP2_IN, i2c_receive_buffer + sizeof(usbthing_i2c_transfer_s), config->num_write, i2c_data_sent_callback);
            break;
        case USBTHING_I2C_MODE_READ:
            I2C_read(config->address, config->num_read, i2c_transmit_buffer);
            USBD_Write(EP2_IN, i2c_transmit_buffer, config->num_read, i2c_data_sent_callback);
            break;
        case USBTHING_I2C_MODE_WRITE_READ:
            I2C_write_read(config->address, config->num_write, i2c_receive_buffer + sizeof(usbthing_i2c_transfer_s), config->num_read, i2c_transmit_buffer);
            USBD_Write(EP2_IN, i2c_transmit_buffer, config->num_read, i2c_data_sent_callback);
            break;
        }

    } else {
        //TODO: handle errors

        //Restart EP_OUT
        USBD_Read(EP1_OUT, i2c_receive_buffer, BUFFERSIZE, i2c_data_receive_callback);
    }

    return USB_STATUS_OK;
}
