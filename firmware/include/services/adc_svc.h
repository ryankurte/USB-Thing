
#ifndef ADC_SVC_H
#define ADC_SVC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_usb.h"

int adc_handle_setup(const USB_Setup_TypeDef *setup);

#ifdef __cplusplus
}
#endif

#endif

