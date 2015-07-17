
#ifndef ADC_SVC_H
#define ADC_SVC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_usb.h"

int adc_cb_configure(const USB_Setup_TypeDef *setup);
int adc_cb_set(const USB_Setup_TypeDef *setup);

#ifdef __cplusplus
}
#endif

#endif

