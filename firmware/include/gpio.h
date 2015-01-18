
#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

enum gpio_mode_e {
	GPIO_MODE_INPUT,
	GPIO_MODE_OUTPUT
};

enum gpio_pin_e {
	GPIO0 = 0,
	GPIO1 = 1,
	GPIO2 = 2,
	GPIO3 = 3
};

enum led_pin_e {
	LED0 = 0,
	LED1 = 1
};

extern void GPIO_init();

extern void GPIO_configure(int pin, bool output, bool pull_enabled, bool pull_up);

extern void GPIO_set(int pin, bool value);

extern void GPIO_led_set(int led, bool value);

extern bool GPIO_get(int pin);

#endif
