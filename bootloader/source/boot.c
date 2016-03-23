/**************************************************************************//**
 * @file
 * @brief Boot Loader
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
#include "boot.h"
#include "em_usb.h"

bool BOOT_checkFirmwareIsValid(void)
{
  uint32_t pc;

#if 1
  pc = *((uint32_t *) BOOTLOADER_SIZE + 1);

  if (pc < MAX_SIZE_OF_FLASH)
    return true;
#endif
  return false;
}

void BOOT_jump(uint32_t sp, uint32_t pc)
{
  (void) sp;
  (void) pc;

  /* Set new MSP, PSP based on SP (r0)*/
  __asm("msr msp, r0");
  __asm("msr psp, r0");

  /* Jump to PC (r1)*/
  __asm("mov pc, r1");
}


/**************************************************************************//**
 * @brief Boots the application
 *****************************************************************************/
void BOOT_boot(void)
{
  uint32_t pc, sp;

  /* Reset all used registers to their default value. */

  /* Disable all interrupts. */
  NVIC->ICER[0] = 0xFFFFFFFF;
  NVIC->ICER[1] = 0xFFFFFFFF;

  /* Disable USB */
  USBD_Stop();

  /* Reset memory system controller settings. */
  MSC->READCTRL  = _MSC_READCTRL_RESETVALUE;
  MSC->WRITECTRL = _MSC_WRITECTRL_RESETVALUE;
  MSC->LOCK = 0;

  /* Reset GPIO settings. */
  GPIO->ROUTE = _GPIO_ROUTE_RESETVALUE;
  GPIO->P[4].MODEH = _GPIO_P_MODEH_RESETVALUE;
  GPIO->P[4].DOUT  = _GPIO_P_DOUT_RESETVALUE;
  GPIO->P[5].MODEL = _GPIO_P_MODEL_RESETVALUE;
  GPIO->P[5].DOUT  = _GPIO_P_DOUT_RESETVALUE;

  /* Reset DMA controller settings. */
  DMA->CONFIG     = _DMA_CONFIG_RESETVALUE;
  DMA->CTRLBASE   = _DMA_CTRLBASE_RESETVALUE;
  DMA->CH[0].CTRL = _DMA_CH_CTRL_RESETVALUE;
  DMA->CHENC      = 0xFFFFFFFF;

  /* Reset TIMER0 settings. */
  TIMER0->CMD        = TIMER_CMD_STOP;
  TIMER0->TOP        = _TIMER_TOP_RESETVALUE;
  TIMER0->CTRL       = _TIMER_CTRL_RESETVALUE;
  TIMER0->CC[0].CTRL = _TIMER_CC_CTRL_RESETVALUE;

  /* Reset TIMER1 settings. */
  AUTOBAUD_TIMER->CMD        = TIMER_CMD_STOP;
  AUTOBAUD_TIMER->TOP        = _TIMER_TOP_RESETVALUE;
  AUTOBAUD_TIMER->IEN        = _TIMER_IEN_RESETVALUE;
  AUTOBAUD_TIMER->IFC        = 0xFFFFFFFF;
  AUTOBAUD_TIMER->CC[0].CTRL = _TIMER_CC_CTRL_RESETVALUE;

  /* Reset RTC settings. */
  RTC->IEN    = _RTC_IEN_RESETVALUE;
  RTC->COMP0  = _RTC_COMP0_RESETVALUE;
  RTC->CTRL   = _RTC_CTRL_RESETVALUE;

  /* Reset UART settings. */
  BOOTLOADER_USART->ROUTE  = _USART_ROUTE_RESETVALUE;
  BOOTLOADER_USART->CLKDIV = _USART_CLKDIV_RESETVALUE;
  BOOTLOADER_USART->CMD    = USART_CMD_RXDIS | USART_CMD_TXDIS;

  /* Wait for LF peripheral syncronization. */
  while (RTC->SYNCBUSY & _RTC_SYNCBUSY_MASK);
  while (CMU->SYNCBUSY & CMU_SYNCBUSY_LFACLKEN0);

  /* Switch to default cpu clock. */
  CMU->CMD      = CMU_CMD_HFCLKSEL_HFRCO;
  CMU->OSCENCMD = CMU_OSCENCMD_HFXODIS | CMU_OSCENCMD_LFRCODIS;

  /* Reset clock registers used. */
  CMU->HFCORECLKEN0 = _CMU_HFCORECLKEN0_RESETVALUE;
  CMU->HFPERCLKDIV  = _CMU_HFPERCLKDIV_RESETVALUE;
  CMU->HFPERCLKEN0  = _CMU_HFPERCLKEN0_RESETVALUE;
  CMU->LFCLKSEL     = _CMU_LFCLKSEL_RESETVALUE;
  CMU->LFACLKEN0    = _CMU_LFACLKEN0_RESETVALUE;

  /* Set new vector table pointer */
  SCB->VTOR = (uint32_t)BOOTLOADER_SIZE;

  /* Read new SP and PC from vector table */
  sp = *((uint32_t *)BOOTLOADER_SIZE    );
  pc = *((uint32_t *)BOOTLOADER_SIZE + 1);

  BOOT_jump(sp, pc);
}
