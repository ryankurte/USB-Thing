/**************************************************************************//**
 * @file
 * @brief USB/USART0 bootloader 1. level loader.
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
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "em_device.h"
#include "boot.h"

const
#include "bootld.h"

extern void* __isr_vector;
uint32_t vector_size = sizeof(__isr_vector);

/**************************************************************************//**
 * The main entry point.
 *****************************************************************************/
int main(void)
{
	__set_MSP( ( 0x20000000 + sizeof( bootloader ) + 0x400 ) & 0xFFFFFFF0 );

	/* Load the entire bootloader into SRAM. */
	memcpy( (void*)0x20000000, bootloader, sizeof( bootloader ) );

	/* Start executing the bootloader. */
	BOOT_jump( *(uint32_t*)0x20000000, *(uint32_t*)0x20000004 );
}
