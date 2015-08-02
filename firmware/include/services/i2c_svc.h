
#ifndef I2C_SVC_H
#define I2C_SVC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "em_usb.h"

void i2c_start();
int i2c_handle_setup(const USB_Setup_TypeDef *setup);
int i2c_data_sent_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
int i2c_data_receive_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);

#ifdef __cplusplus
}
#endif

#endiI2C