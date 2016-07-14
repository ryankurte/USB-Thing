

#ifndef USBTHING_H
#define USBTHING_H

#include <stdint.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct usbthing_s * usbthing_t;

int USBTHING_init();
void USBTHING_close();

int USBTHING_list_devices(uint16_t vid_filter, uint16_t pid_filter);

int USBTHING_connect(usbthing_t *usbthing, uint16_t vid_filter, uint16_t pid_filter);

int USBTHING_disconnect(usbthing_t *usbthing);

int USBTHING_get_firmware_version(usbthing_t usbthing, int length, char *version);

int USBTHING_led_set(usbthing_t usbthing, int led, int value);

int USBTHING_gpio_configure(usbthing_t usbthing, int pin, int output, int pull_enabled, int pull_up);

int USBTHING_gpio_set(usbthing_t usbthing, int pin, int value);

int USBTHING_gpio_get(usbthing_t usbthing, int pin, int *value);

int USBTHING_gpio_get_int(usbthing_t usbthing, int pin, int *value);

int USBTHING_pwm_configure(usbthing_t usbthing, unsigned int frequency);

int USBTHING_pwm_enable(usbthing_t usbthing, int channel, int enable);

int USBTHING_pwm_set(usbthing_t usbthing, int channel, int duty_cycle);

int USBTHING_dac_configure(usbthing_t usbthing);

int USBTHING_dac_set(usbthing_t usbthing, unsigned int enable, float value);

int USBTHING_adc_configure(usbthing_t usbthing, unsigned int reference);

int USBTHING_adc_get(usbthing_t usbthing, int channel, float *value);

int USBTHING_spi_configure(usbthing_t usbthing, unsigned int speed, int mode);

int USBTHING_spi_transfer(usbthing_t usbthing, int length, unsigned char *data_out, unsigned char *data_in);

int USBTHING_spi_close(usbthing_t usbthing);

int USBTHING_i2c_configure(usbthing_t usbthing, int speed);

int USBTHING_i2c_write(usbthing_t usbthing,
                       int address,
                       int length_out, unsigned char *data_out);

int USBTHING_i2c_read(usbthing_t usbthing,
                      int address,
                      int length_in, unsigned char *data_in);

int USBTHING_i2c_write_read(usbthing_t usbthing,
                            int address,
                            int length_out, unsigned char *data_out,
                            int length_in, unsigned char *data_in);

#ifdef __cplusplus
}
#endif

#endif
