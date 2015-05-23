/***************************************************************************//**
 * @file
 * @brief USB protocol stack library, application supplied configuration options.
 * @author Energy Micro AS
 * @version 1.02
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2011 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * This source code is the property of Energy Micro AS. The source and compiled
 * code may only be used on Energy Micro "EFM32" microcontrollers.
 *
 * This copyright notice may not be removed from the source code nor changed.
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
 ******************************************************************************/
#ifndef __USBCONFIG_H
#define __USBCONFIG_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define USB_DEVICE

/****************************************************************************
**                                                                         **
** Specify number of endpoints used (in addition to EP0).                  **
**                                                                         **
*****************************************************************************/
#define NUM_EP_USED 3

/****************************************************************************
**                                                                         **
** Specify number of application timers you need.                          **
**                                                                         **
*****************************************************************************/
#define NUM_APP_TIMERS 0

/****************************************************************************
**                                                                         **
** Configure serial port debug output.                                     **
**                                                                         **
*****************************************************************************/

#if defined( BL_DEBUG )
  /* Define a function for transmitting a single char on the serial port. */
  extern int RETARGET_WriteChar(char c);
  #define USER_PUTCHAR  RETARGET_WriteChar

  /* Debug USB API functions (illegal input parameters etc.) */
  #define DEBUG_USB_API

  /* Include the printf function. */
  #define USB_USE_PRINTF
#endif

#ifdef __cplusplus
}
#endif

#endif /* __USBCONFIG_H */
