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

#include "callbacks.h"

#include "protocol.h"
#include "platform.h"
#include "gpio.h"
#include "version.h"

#define BUFFERSIZE 500

/* Buffer to receive incoming messages. Needs to be
 * WORD aligned and an integer number of WORDs large */
STATIC_UBUF(receiveBuffer, BUFFERSIZE);

/* Counter to increase when receiving a 'tick' message */
int tickCounter = 0;

extern uint8_t button0message[];
extern uint8_t button1message[];

EFM32_ALIGN(4)
uint8_t firmware_version[] = SOFTWARE_VERSION;
EFM32_ALIGN(4)
uint8_t pin_value[USBTHING_CMD_GPIO_GET_SIZE];

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

    } else if (newState == USBD_STATE_CONFIGURED) {
        /* Start waiting for the 'tick' messages */
        USBD_Read(EP_OUT, receiveBuffer, BUFFERSIZE, dataReceivedCallback);

    } else if ( newState != USBD_STATE_SUSPENDED ) {

    }
}

int getFirmware(const USB_Setup_TypeDef *setup)
{
    int res = USB_STATUS_REQ_ERR;

    if ( ( setup->wIndex      != 0                             ) ||
         ( setup->wLength     != USBTHING_FIRMWARE_MAX_SIZE    ) ||
         ( setup->wValue      != 0                             ) ||
         ( setup->Direction   != USB_SETUP_DIR_IN              ) ||
         ( setup->Recipient   != USB_SETUP_RECIPIENT_DEVICE    )) {
        return USB_STATUS_REQ_ERR;
    }

    res = USBD_Write(0, firmware_version, sizeof(firmware_version), NULL);

    return res;
}

int setLed(const USB_Setup_TypeDef* setup) {
    int res = USB_STATUS_REQ_ERR;

    if ( ( setup->wLength     != USBTHING_CMD_LED_SET_SIZE     ) ||
         ( setup->Direction   != USB_SETUP_DIR_OUT              ) ||
         ( setup->Recipient   != USB_SETUP_RECIPIENT_DEVICE    )) {
        return USB_STATUS_REQ_ERR;
    }

    GPIO_led_set(setup->wValue, setup->wIndex);

    return USB_STATUS_OK;
}

int configureGPIO(const USB_Setup_TypeDef *setup)
{
    if ( ( setup->wLength     != USBTHING_CMD_GPIO_CFG_SIZE    ) ||
         ( setup->Direction   != USB_SETUP_DIR_OUT              ) ||
         ( setup->Recipient   != USB_SETUP_RECIPIENT_DEVICE    )) {
        return USB_STATUS_REQ_ERR;
    }

    uint8_t pin = setup->wValue;
    bool output = ((setup->wIndex & USBTHING_GPIO_CFG_MODE_OUTPUT) != 0) ? true : false;
    bool pull_enabled = ((setup->wIndex & USBTHING_GPIO_CFG_PULL_ENABLE) != 0) ? true : false;
    bool pull_direction = ((setup->wIndex & USBTHING_GPIO_CFG_PULL_HIGH) != 0) ? true : false;

    GPIO_configure(pin, output, pull_enabled, pull_direction);

    return USB_STATUS_OK;
}

int setGPIO(const USB_Setup_TypeDef *setup)
{
    if ( ( setup->wLength     != USBTHING_CMD_GPIO_GET_SIZE    ) ||
         ( setup->Direction   != USB_SETUP_DIR_OUT             ) ||
         ( setup->Recipient   != USB_SETUP_RECIPIENT_DEVICE    )) {
        return USB_STATUS_REQ_ERR;
    }

    GPIO_set(setup->wValue, setup->wIndex);

    return USB_STATUS_OK;
}

int getGPIO(const USB_Setup_TypeDef *setup)
{
    int res = USB_STATUS_REQ_ERR;

    if ( ( setup->wLength     != USBTHING_CMD_GPIO_GET_SIZE    ) ||
         ( setup->Direction   != USB_SETUP_DIR_IN              ) ||
         ( setup->Recipient   != USB_SETUP_RECIPIENT_DEVICE    )) {
        return USB_STATUS_REQ_ERR;
    }

    uint8_t pin = setup->wValue;
    pin_value[0] = GPIO_get(pin);

    //TODO: respond
    res = USBD_Write(0, pin_value, USBTHING_CMD_FIRMWARE_GET_SIZE, NULL);

    return res;
}

int setupCmd(const USB_Setup_TypeDef *setup)
{
    //TODO: handle commands

    switch (setup->bRequest) {
    case USBTHING_CMD_NOP:
        __asm("nop");
        return USB_STATUS_OK;

    case USBTHING_CMD_FIRMWARE_GET:
        getFirmware(setup);
        return USB_STATUS_OK;

    case USBTHING_CMD_LED_SET:
        setLed(setup);
        return USB_STATUS_OK;

    case USBTHING_CMD_GPIO_CFG:
        return configureGPIO(setup);

    case USBTHING_CMD_GPIO_SET:
        return setGPIO(setup);

    case USBTHING_CMD_GPIO_GET:
        return getGPIO(setup);

    }

    //Signal command was not handled
    return USB_STATUS_REQ_UNHANDLED;
}

/**********************************************************
 * Called when data is sent on the IN endpoint.
 *
 * @param status
 *   The transfer status. Should be USB_STATUS_OK if the
 *   transfer completed successfully.
 *
 * @param xferred
 *   The number of bytes actually transmitted
 *
 * @param remaining
 *   The number of bytes remaining (not transferred)
 **********************************************************/
int dataSentCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
    /* Remove warnings for unused variables */
    (void)xferred;
    (void)remaining;

    if ( status != USB_STATUS_OK ) {
        /* Handle error */
    }
    return USB_STATUS_OK;
}

/**********************************************************
 * Called when data is received on the OUT endpoint.
 * This function will increase the counter and update
 * the LCD display when it receives a 'tick' message
 * to let the user know that the message was received
 * (only on STK example).
 *
 * @param status
 *   The transfer status. Should be USB_STATUS_OK if the
 *   transfer completed successfully.
 *
 * @param xferred
 *   The number of bytes actually received
 *
 * @param remaining
 *   The number of bytes remaining (not transferred)
 **********************************************************/
int dataReceivedCallback(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
    /* Remove warnings for unused variables */
    (void)xferred;
    (void)remaining;

    printf("\nReceived %s", receiveBuffer);

    /* Check status to verify that the transfer has completed successfully */
    if ( status == USB_STATUS_OK ) {
        /* Check if we received the 'tick' message */
        if ( strcmp((char *)receiveBuffer, "tick") == 0 ) {
            /* Increase the number on the LCD */
#ifdef STK
            SegmentLCD_Number(++tickCounter);
#endif
        }

        /* Prepare to accept the next message */
        USBD_Read(EP_OUT, receiveBuffer, BUFFERSIZE, dataReceivedCallback);
    } else {
        /* Handle errors here.  */
    }

    return USB_STATUS_OK;
}
