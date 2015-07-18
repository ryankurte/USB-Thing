
#ifndef SPI_SVC_H
#define SPI_SVC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_usb.h"

void spi_start();
int spi_handle_setup(const USB_Setup_TypeDef *setup);
int spi_data_sent_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
int spi_data_receive_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);

#ifdef __cplusplus
}
#endif

#endif

