
#ifndef DAC_SVC_H
#define DAC_SVC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_usb.h"

int dac_cb_configure(const USB_Setup_TypeDef *setup);
int dac_cb_enable(const USB_Setup_TypeDef *setup);
int dac_cb_set(const USB_Setup_TypeDef *setup);

#ifdef __cplusplus
}
#endif

#endif

