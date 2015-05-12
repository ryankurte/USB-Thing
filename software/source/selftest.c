
#include <stdint.h>
#include <stdio.h>

#include "usbthing.h"

static int test_dac_adc(struct usbthing_s* usbthing);
static int test_gpio(struct usbthing_s* usbthing);
static int test_spi(struct usbthing_s* usbthing);
static int test_i2c(struct usbthing_s* usbthing);

int self_test(struct usbthing_s* usbthing)
{
	int res;

	res = test_gpio(usbthing);
	if (res < 0) {
		printf("GPIO test failed: %d\r\n", res);
		return -2;
	}

#if 0
	res = test_dac_adc(usbthing);
	if (res < 0) {
		printf("DAC -> ADC test failed: %d\r\n", res);
		return -1;
	}
#endif

	return 0;
}

static int test_dac_adc(struct usbthing_s* usbthing)
{
	char c;

	printf("DAC and ADC test\r\n");
	printf("Connect DAC to ADC port 0 and press any key to continue\r\n");
	getchar();

	//USBTHING_dac_configure();

	//USBTHING_adc_configure();

	//USBTHING_dac_write(0);

	//USBTHING_adc_read(0);

	//TODO: compare values

	//TODO: repeat

	return 0;
}

static int test_gpio(struct usbthing_s* usbthing)
{
	bool value;
	int res;

	printf("GPIO test\r\n");
	printf("Connect pins GPIO0 to GPIO1 and GPIO2 to GPIO3 and press any key to continue\r\n");
	getchar();

	USBTHING_gpio_configure(usbthing, 0, true, false, false);
	USBTHING_gpio_configure(usbthing, 1, false, false, false);
	USBTHING_gpio_configure(usbthing, 2, true, false, false);
	USBTHING_gpio_configure(usbthing, 3, false, false, false);

	//First pair, low
	res = USBTHING_gpio_set(usbthing, 0, false);
	if (res < 0) {
		return -1;
	}
	res = USBTHING_gpio_get(usbthing, 0, &value);
	if (res < 0) {
		return -2;
	}
	if (value != false) {
		return -3;
	}

	//First pair, high
	res = USBTHING_gpio_set(usbthing, 0, true);
	if (res < 0) {
		return -4;
	}
	res = USBTHING_gpio_get(usbthing, 0, &value);
	if (res < 0) {
		return -5;
	}
	if (value != true) {
		return -6;
	}


	return 0;
}

static int test_spi(struct usbthing_s* usbthing)
{
	printf("SPI test\r\n");
	printf("Connect SPI MISO and MOSI pins and press any key to continue\r\n");
	getchar();

	//USBTHING_SPI_configure()

	//USBTHING_SPI_writeread()

	//TODO: compare sent and response values

	//TODO: check CS

	//TODO: check CLK

	return 0;
}

static int test_i2c(struct usbthing_s* usbthing)
{

	//I don't even know how to start this

	return 0;
}


