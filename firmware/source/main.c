/**************************************************************************//**
 * @file main_stk.c
 * @brief USB Device Example
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
#include "em_emu.h"

#include "callbacks.h"
#include "descriptors.h"
#include "platform.h"
#include "gpio.h"

#define DEBUG_USB

uint32_t INT_LockCnt;

extern void __libc_init_array();

/* Messages to send when the user presses buttons on the kit */
EFM32_ALIGN(4)
uint8_t button0message[] = "PB0 pressed!";
EFM32_ALIGN(4)
uint8_t button1message[] = "PB1 pressed!";


int main(void)
{
    /* Chip errata */
    CHIP_Init();

    /* Enable HFXO */
    //CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

    CMU_ClockEnable(cmuClock_HFPER, true);

    printf("\nStarting USB Device...\n");

    /* Set up GPIO interrupts */
    GPIO_init();

    /* Start USB stack. Callback routines in callbacks.c will be called
     * when connected to a host.  */
    USBD_Init(&initstruct);;

    /*
     * When using a debugger it is practical to uncomment the following three
     * lines to force host to re-enumerate the device.
     */
#ifdef DEBUG_USB
    USBD_Disconnect();
    USBTIMER_DelayMs( 1000 );
    USBD_Connect();
#endif

    while (1) {
#if 0
        if ( USBD_SafeToEnterEM2() ) {
            /* Enter EM2 when in suspend or disconnected */
            EMU_EnterEM2(true);
        } else {
            /* When USB is active we can sleep in EM1. */
            EMU_EnterEM1();
        }
#endif
    }
}

int RETARGET_WriteChar(char c)
{

    return 0;
}