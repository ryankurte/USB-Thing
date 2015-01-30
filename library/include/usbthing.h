

#ifndef USBTHING_H
#define USBTHING_H

#include <stdint.h>
#include <stdbool.h>

#include "libusb-1.0/libusb.h"

struct usbthing_s {
    libusb_device_handle *handle;
};

int USBTHING_init();
void USBTHING_close();

int USBTHING_list_devices(uint16_t vid_filter, uint16_t pid_filter);

int USBTHING_connect(struct usbthing_s *usbthing, uint16_t vid_filter, uint16_t pid_filter);

int USBTHING_disconnect(struct usbthing_s *usbthing);

int USBTHING_get_firmware_version(struct usbthing_s *usbthing, char *version, int *length);

int USBTHING_led_set(struct usbthing_s *usbthing, int led, bool value);

int USBTHING_gpio_configure(struct usbthing_s *usbthing, int pin, bool output, bool pull_enabled, bool pull_up);

int USBTHING_gpio_set(struct usbthing_s *usbthing, int pin, bool value);

int USBTHING_gpio_get(struct usbthing_s *usbthing, int pin, bool *value);

#endif
