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
#include <stdbool.h>
#include "em_device.h"
#include "em_usb.h"
#include "xmodem.h"
#include "bootldio.h"
#include "config.h"

#define USB_BUF_SIZ 256

static bool useUsb = false;

STATIC_UBUF( usbBuffer, USB_BUF_SIZ );

static uint32_t           usbXferCnt;
static volatile bool      usbXferDone;
static USB_Status_TypeDef usbXferStatus;

/**************************************************************************//**
 * @brief
 *    Callback function called whenever a packet with data has been
 *    transferred on USB.
 *****************************************************************************/
static int UsbDataXferred( USB_Status_TypeDef status,
                           uint32_t xferred, uint32_t remaining )
{
  (void)remaining;            /* Unused parameter */

  usbXferStatus = status;
  usbXferCnt    = xferred;
  usbXferDone   = true;

  return USB_STATUS_OK;
}

/***************************************************************************//**
 * @brief
 *   Prints an int in hex.
 *
 * @param integer
 *   The integer to be printed.
 ******************************************************************************/
void BOOTLDIO_printHex( uint32_t integer )
{
  uint8_t c;
  int     i, j, digit;

  for ( i = 7, j = 0; i >= 0; i--, j++ )
  {
    digit = (integer >> (i * 4)) & 0xf;
    if (digit < 10)
    {
      c = digit + 0x30;
    }
    else
    {
      c = digit + 0x37;
    }
    if ( useUsb )
    {
      usbBuffer[ j ] = c;
    }
    else
    {
      BOOTLDIO_txByte(c);
    }
  }

  if ( useUsb )
  {
    usbBuffer[ j ] = '\0';
    BOOTLDIO_printString( usbBuffer );
  }
}

/**************************************************************************//**
 * @brief Get single byte from USART or USB
 *****************************************************************************/
uint8_t BOOTLDIO_rxByte( void )
{
  uint8_t  retVal;
  uint32_t timer = 2000000;

  usbXferDone = false;
  USBD_Read( EP_DATA_OUT, usbBuffer, USB_BUF_SIZ, UsbDataXferred );
  while ( !usbXferDone ) {}
  retVal = usbXferStatus == USB_STATUS_OK ? usbBuffer[0] : 0;

  return retVal;
}

/**************************************************************************//**
 * @brief Get an XMODEM packet from USB with optional timeout.
 *
 * @param[in] p
 *   Pointer to XMODEM storage space.
 *
 * @param[in] timeout
 *   Transmission timeout in milliseconds, no timeout if zero.
 *
 * @return
 *   True if a transmission took place.
 *****************************************************************************/
bool BOOTLDIO_getPacket( XMODEM_packet *p, int timeout )
{
  usbXferDone = false;
  USBD_Read( EP_DATA_OUT, usbBuffer, USB_BUF_SIZ, UsbDataXferred );

  if ( timeout )
  {
    while ( !usbXferDone && --timeout )
    {
      USBTIMER_DelayMs( 1 );
    }

    if ( timeout <= 0 )
    {
      USBD_AbortTransfer( EP_DATA_OUT );
      return false;
    }
  }
  else
  {
    while ( !usbXferDone ) {}
  }

  /*
   * Copy data at p+1 so that data payload becomes even aligned.
   * See definition of XMODEM_packet.
   */
  memcpy( (uint8_t*)p + 1, usbBuffer, usbXferCnt );
  return true;
}

/**************************************************************************//**
 * @brief Transmit single byte to USART or USB
 *****************************************************************************/
int BOOTLDIO_txByte( uint8_t data )
{
  usbBuffer[ 0 ] = data;
  usbXferDone = false;
  USBD_Write( EP_DATA_IN, usbBuffer, 1, UsbDataXferred );
  while ( !usbXferDone ) {}
  return usbXferStatus == USB_STATUS_OK ? (int)data : 0;

  return (int)data;
}

/**************************************************************************//**
 * @brief Transmit null-terminated string to USART or USB
 *****************************************************************************/
void BOOTLDIO_printString( const uint8_t *string )
{
  int len;

  len = strlen( (char*)string );
  memcpy( usbBuffer, string, len );
  usbXferDone = false;
  USBD_Write( EP_DATA_IN, usbBuffer, len, UsbDataXferred );
  while ( !usbXferDone ) {}
}