
#include "gpio.h"

#include "em_cmu.h"
#include "em_gpio.h"

#include "platform.h"

void GPIO_init()
{
    //Start clock
    CMU_ClockEnable(cmuClock_GPIO, true);

	//Initialize LEDs
	GPIO_PinModeSet(LED0_PORT, LED0_PIN, gpioModePushPull, 0);
    GPIO_PinModeSet(LED1_PORT, LED1_PIN, gpioModePushPull, 0);

    //Initialize GPIOs as inputs
    GPIO_PinModeSet(GPIO0_PORT, GPIO0_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(GPIO1_PORT, GPIO1_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(GPIO2_PORT, GPIO2_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(GPIO3_PORT, GPIO3_PIN, gpioModeInput, 0);
}

void GPIO_configure(int pin, bool output, bool pull_enabled, bool pull_up)
{
    //TODO: enable pull
    switch (pin) {
    case GPIO0:
        if (output == true) {
            GPIO_PinModeSet(GPIO0_PORT, GPIO0_PIN, gpioModePushPull, 0);
        } else {
            GPIO_PinModeSet(GPIO0_PORT, GPIO0_PIN, gpioModeInput, 0);
        }
        return;
    case GPIO1:
        if (output == true) {
            GPIO_PinModeSet(GPIO0_PORT, GPIO0_PIN, gpioModePushPull, 0);
        } else {
            GPIO_PinModeSet(GPIO0_PORT, GPIO0_PIN, gpioModeInput, 0);
        }
        return;
    case GPIO2:
        if (output == true) {
            GPIO_PinModeSet(GPIO0_PORT, GPIO0_PIN, gpioModePushPull, 0);
        } else {
            GPIO_PinModeSet(GPIO0_PORT, GPIO0_PIN, gpioModeInput, 0);
        }
        return;
    case GPIO3:
        if (output == true) {
            GPIO_PinModeSet(GPIO0_PORT, GPIO0_PIN, gpioModePushPull, 0);
        } else {
            GPIO_PinModeSet(GPIO0_PORT, GPIO0_PIN, gpioModeInput, 0);
        }
        return;
    }
}

void GPIO_led_set(int led, bool value)
{
    switch (led) {
    case LED0:
        if (value == true) {
            GPIO_PinOutSet(LED0_PORT, LED0_PIN);
        } else {
            GPIO_PinOutClear(LED0_PORT, LED0_PIN);
        }
        return;
    case LED1:
        if (value == true) {
            GPIO_PinOutSet(LED1_PORT, LED1_PIN);
        } else {
            GPIO_PinOutClear(LED1_PORT, LED1_PIN);
        }
        return;
    }
}

void GPIO_set(int pin, bool value)
{
    switch (pin) {
    case GPIO0:
        if (value == true) {
            GPIO_PinOutSet(GPIO0_PORT, GPIO0_PIN);
        } else {
            GPIO_PinOutClear(GPIO0_PORT, GPIO0_PIN);
        }
        return;
    case GPIO1:
        if (value == true) {
            GPIO_PinOutSet(GPIO1_PORT, GPIO1_PIN);
        } else {
            GPIO_PinOutClear(GPIO1_PORT, GPIO1_PIN);
        }
        return;
    case GPIO2:
        if (value == true) {
            GPIO_PinOutSet(GPIO2_PORT, GPIO2_PIN);
        } else {
            GPIO_PinOutClear(GPIO2_PORT, GPIO2_PIN);
        }
        return;
    case GPIO3:
        if (value == true) {
            GPIO_PinOutSet(GPIO3_PORT, GPIO3_PIN);
        } else {
            GPIO_PinOutClear(GPIO3_PORT, GPIO3_PIN);
        }
        return;
    }
}

bool GPIO_get(int pin)
{
    switch (pin) {
    case GPIO0:
        return GPIO_PinInGet(GPIO0_PORT, GPIO0_PIN);
    case GPIO1:
        return GPIO_PinInGet(GPIO1_PORT, GPIO1_PIN);
    case GPIO2:
        return GPIO_PinInGet(GPIO2_PORT, GPIO2_PIN);
    case GPIO3:
        return GPIO_PinInGet(GPIO3_PORT, GPIO3_PIN);
    default:
        return 0;
    }
}

/**********************************************************
 * Interrupt handler for push button 0
 **********************************************************/
void GPIO_ODD_IRQHandler(void)
{
	//TODO
    //USBD_Write(EP_IN, button0message, sizeof(button0message), dataSentCallback);
}

/**********************************************************
 * Interrupt handler for push button 1
 **********************************************************/
void GPIO_EVEN_IRQHandler(void)
{
    //TODO
}