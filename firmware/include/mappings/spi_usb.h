
#ifndef SPI_USB_H
#define SPI_USB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_usb.h"

void spi_cb_start();
int spi_cb_configure(const USB_Setup_TypeDef *setup);
int spi_cb_data_sent(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
int spi_cb_data_receive(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);

#ifdef __cplusplus
}
#endif

#endif

