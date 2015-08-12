
#ifndef DAC_SVC_H
#define DAC_SVC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_usb.h"

int dac_handle_setup(const USB_Setup_TypeDef *setup);

#ifdef __cplusplus
}
#endif

#endif

