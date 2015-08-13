
#ifndef spi_svc_SVC_H
#define spi_svc_SVC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_usb.h"

void spi_svc_start();
int spi_svc_handle_setup(const USB_Setup_TypeDef *setup);

#ifdef __cplusplus
}
#endif

#endif

