/**************************************************************************//**
 * @file
 * @brief IO code, USART or USB, for the EFM32 bootloader
 * @author Energy Micro AS
 * @version 1.02
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2011 Energy Micro AS, http://www.energymicro.com</b>
 ******************************************************************************
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
 *****************************************************************************/

#ifndef _BOOTLDIO_H
#define _BOOTLDIO_H

void    BOOTLDIO_printHex(      uint32_t integer      );
int     BOOTLDIO_txByte(        uint8_t data          );
uint8_t BOOTLDIO_rxByte(        void                  );
void    BOOTLDIO_printString(   const uint8_t *string );
void    BOOTLDIO_usartInit(     uint32_t clkdiv       );
void    BOOTLDIO_setMode(       bool usb              );
bool    BOOTLDIO_usbMode(       void                  );
bool    BOOTLDIO_getPacket(     XMODEM_packet *p, int timeout );

#endif
