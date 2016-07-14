
#ifndef I2C_SVC_H
#define I2C_SVC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_usb.h"

void i2c_svc_start();
int i2c_svc_handle_setup(const USB_Setup_TypeDef *setup);

#ifdef __cplusplus
}
#endif

#endif //I2C_SVC_H