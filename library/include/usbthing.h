

#ifndef USBTHING_H
#define USBTHING_H

#include <stdint.h>
#include <stdint.h>

#include "libusb-1.0/libusb.h"

#include "protocol.h"

struct usbthing_s {
	libusb_device_handle *handle;
};

int USBTHING_init();
void USBTHING_close();

int USBTHING_list_devices(uint16_t vid_filter, uint16_t pid_filter);

int USBTHING_connect(struct usbthing_s *usbthing, uint16_t vid_filter, uint16_t pid_filter);

int USBTHING_disconnect(struct usbthing_s *usbthing);

int USBTHING_get_firmware_version(struct usbthing_s *usbthing, int length, char *version);

int USBTHING_led_set(struct usbthing_s *usbthing, int led, int value);

int USBTHING_gpio_configure(struct usbthing_s *usbthing, int pin, int output, int pull_enabled, int pull_up);

int USBTHING_gpio_set(struct usbthing_s *usbthing, int pin, int value);

int USBTHING_gpio_get(struct usbthing_s *usbthing, int pin, int *value);

int USBTHING_gpio_get_int(struct usbthing_s *usbthing, int pin, int *value);

int USBTHING_pwm_configure(struct usbthing_s *usbthing, unsigned int frequency);

int USBTHING_pwm_enable(struct usbthing_s *usbthing, int channel, int enable);

int USBTHING_pwm_set(struct usbthing_s *usbthing, int channel, int duty_cycle);

int USBTHING_dac_configure(struct usbthing_s *usbthing);

int USBTHING_dac_enable(struct usbthing_s *usbthing, int enable);

int USBTHING_dac_set(struct usbthing_s *usbthing, unsigned int value);

int USBTHING_adc_configure(struct usbthing_s *usbthing, unsigned int reference);

int USBTHING_adc_get(struct usbthing_s *usbthing, int channel, unsigned int *value);

int USBTHING_spi_configure(struct usbthing_s *usbthing, unsigned int speed, int mode);

int USBTHING_spi_transfer(struct usbthing_s *usbthing, unsigned char *data_out, unsigned char *data_in, int length);

int USBTHING_spi_close(struct usbthing_s *usbthing);

int USBTHING_i2c_configure(struct usbthing_s *usbthing, int mode);

int USBTHING_i2c_write(struct usbthing_s *usbthing,
                       int address,
                       int length_out, unsigned char *data_out);

int USBTHING_i2c_read(struct usbthing_s *usbthing,
                      int address,
                      int length_in, unsigned char *data_in);

int USBTHING_i2c_write_read(struct usbthing_s *usbthing,
                            int address,
                            int length_out, unsigned char *data_out,
                            int length_in, unsigned char *data_in);

#endif
