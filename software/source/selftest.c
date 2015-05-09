
#include <stdint.h>
#include <stdio.h>

#include "usbthing.h"

int self_test(struct usbthing_s* usbthing)
{
	//Test connectivity


}

static int test_dac_adc(struct usbthing_s usbthing)
{
	char c;

	printf("DAC and ADC test\r\n");
	printf("Connect DAC to ADC port 0 and press any key to continue\r\n");

	//USBTHING_dac_configure();

	//USBTHING_adc_configure();

	//USBTHING_dac_write(0);

	//USBTHING_adc_read(0);

	//TODO: compare values

	//TODO: repeat

	return 0;
}

static int test_gpio(struct usbthing_s usbthing)
{
	printf("GPIO test\r\n");
	printf("Connect pins GPIO0 to GPIO1 and GPIO2 to GPIO3 and press any key to continue\r\n");

	//GPIO_configure();

	//GPIO_configure();

	//GPIO_write();

	//GPIO_read();

	//TODO: check values

	return 0;
}

static int test_spi(struct usbthing_s usbthing)
{
	printf("SPI test\r\n");
	printf("Connect SPI MISO and MOSI pins and press any key to continue\r\n");

	//USBTHING_SPI_configure()

	//USBTHING_SPI_writeread()

	//TODO: compare sent and response values

	//TODO: check CS

	//TODO: check CLK

	return 0;
}

static int test_i2c(struct usbthing_s usbthing)
{

	//I don't even know how to start this

	return 0;
}


