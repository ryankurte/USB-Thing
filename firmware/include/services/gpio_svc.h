
#ifndef GPIO_SVC_H
#define GPIO_SVC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_usb.h"

int gpio_cb_configure(const USB_Setup_TypeDef *setup);
int gpio_cb_set(const USB_Setup_TypeDef *setup);
int gpio_cb_get(const USB_Setup_TypeDef *setup);

#ifdef __cplusplus
}
#endif

#endif

