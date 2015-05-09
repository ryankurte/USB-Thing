/**************************************************************************//**
 * @file
 * @brief Bootloader flash writing functions.
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
#ifndef FLASH_H
#define FLASH_H

/*
 * Flash programming hardware interface
 *
 */

/* Helper functions */
void FLASH_writeWord(uint32_t address, uint32_t data);
void FLASH_writeBlock(void *block_start,
                      uint32_t offset_into_block,
                      uint32_t count,
                      uint8_t const *buffer);
void FLASH_eraseOneBlock(uint32_t blockStart);
void FLASH_massErase( uint32_t eraseCmd );
void FLASH_init(void);
void FLASH_CalcPageSize(void);

extern uint32_t flashSize;
extern uint32_t flashPageSize;
#endif
