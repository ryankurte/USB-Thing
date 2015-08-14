
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "usbthing.h"

#define SPI_BULK_TEST_SIZE		128

static int test_adc(struct usbthing_s* usbthing, int interactive);
static int test_dac_adc(struct usbthing_s* usbthing, int interactive);
static int test_gpio(struct usbthing_s* usbthing, int interactive);
static int test_spi(struct usbthing_s* usbthing, int interactive);
static int test_spi_bulk(struct usbthing_s* usbthing, int interactive);
static int test_i2c(struct usbthing_s* usbthing, int interactive);

int self_test(struct usbthing_s* usbthing, int interactive)
{
	int res;

	res = test_gpio(usbthing, interactive);
	if (res < 0) {
		printf("GPIO test failed: %d\r\n", res);
	} else {
		printf("GPIO test OK\r\n");
	}

	res = test_spi(usbthing, interactive);
	if (res < 0) {
		printf("SPI test failed: %d\r\n", res);
	} else {
		printf("SPI test OK\r\n");
	}

	res = test_spi_bulk(usbthing, interactive);
	if (res < 0) {
		printf("SPI bulk test failed: %d\r\n", res);
	} else {
		printf("SPI test OK\r\n");
	}

	res = test_adc(usbthing, interactive);
	if (res < 0) {
		printf("ADC test failed: %d\r\n", res);
	} else {
		printf("ADC test OK\r\n");
	}

	res = test_dac_adc(usbthing, interactive);
	if (res < 0) {
		printf("DAC -> ADC test failed: %d\r\n", res);
		return -1;
	}

	return 0;
}

static int test_gpio_pair(struct usbthing_s* usbthing, int in, int out)
{
	int value;
	int res;

	//Configure
	USBTHING_gpio_configure(usbthing, out, 1, 0, 0);
	USBTHING_gpio_configure(usbthing, in, 0, 0, 0);

	//First pair, low
	res = USBTHING_gpio_set(usbthing, out, 0);
	if (res < 0) {
		printf("GPIO pair (%d %d) set output low failed\r\n", in, out);
		return -1;
	}
	res = USBTHING_gpio_get(usbthing, in, &value);
	if (res < 0) {
		printf("GPIO pair (%d %d) read low input failed\r\n", in, out);
		return -2;
	}
	if (value != 0) {
		printf("GPIO pair (%d %d) read low input incorrect\r\n", in, out);
		return -3;
	}

	//First pair, high
	res = USBTHING_gpio_set(usbthing, out, 1);
	if (res < 0) {
		printf("GPIO pair (%d %d) set output high failed\r\n", in, out);
		return -4;
	}
	res = USBTHING_gpio_get(usbthing, in, &value);
	if (res < 0) {
		printf("GPIO pair (%d %d) read high input failed\r\n", in, out);
		return -5;
	}
	if (value == 0) {
		printf("GPIO pair (%d %d) read high input incorrect\r\n", in, out);
		return -6;
	}

	return 0;
}

static int test_gpio(struct usbthing_s* usbthing, int interactive)
{
	int res;

	if (interactive != 0) {
		printf("GPIO test\r\n");
		printf("Connect pins GPIO0 to GPIO1, GPIO2 to GPIO3 and GPIO4 to GPIO5 and press any key to continue\r\n");
		getchar();
	}

	res = test_gpio_pair(usbthing, 0, 1);
	if (res < 0) {
		printf("GPIO test pair 0 failed\r\n");
		return -1;
	}

	res = test_gpio_pair(usbthing, 2, 3);
	if (res < 0) {
		printf("GPIO test pair 1 failed\r\n");
		return -2;
	}

	res = test_gpio_pair(usbthing, 4, 5);
	if (res < 0) {
		printf("GPIO test pair 2 failed\r\n");
		return -3;
	}

	return 0;
}

#define TEST_DATA_SIZE	32

static int test_spi(struct usbthing_s* usbthing, int interactive)
{
	uint8_t data_out[TEST_DATA_SIZE];
	uint8_t data_in[TEST_DATA_SIZE];

	printf("SPI test\r\n");
	if (interactive != 0) {
		printf("Connect SPI MISO and MOSI pins and press any key to continue\r\n");
		getchar();
	}

	USBTHING_spi_configure(usbthing, USBTHING_SPI_SPEED_100KHZ, USBTHING_SPI_CLOCK_MODE0);

	//Repeat test N times
	for (int i = 0; i < 10; i++) {

		for (int j = 0; j < TEST_DATA_SIZE; j++) {
			data_out[j] = rand();
		}

		USBTHING_spi_transfer(usbthing, data_out, data_in, sizeof(data_out));

		//TODO: compare sent and response values

		if (strncmp((const char*)data_out, (const char*)data_in, sizeof(data_out)) != 0) {
			printf("SPI test transfer %d data mismatch\r\n", i);
			printf("out: %s\r\n", data_out);
			printf("in: %s\r\n", data_out);
			return -1;
		}

		usleep(1000);
	}

	printf("SPI test transfers complete\r\n");

	//TODO: check CS

	//TODO: check CLK

	USBTHING_spi_close(usbthing);

	return 0;
}

static int test_spi_bulk(struct usbthing_s* usbthing, int interactive)
{
	uint8_t data_out[SPI_BULK_TEST_SIZE];
	uint8_t data_in[SPI_BULK_TEST_SIZE];

	printf("SPI bulk test\r\n");
	if (interactive != 0) {
		printf("Connect SPI MISO and MOSI pins and press any key to continue\r\n");
		getchar();
	}

	//Configure SPI device
	USBTHING_spi_configure(usbthing, USBTHING_SPI_SPEED_100KHZ, USBTHING_SPI_CLOCK_MODE0);


	for (int i = 0; i < 10; i++) {

		//Data set one
		for (int j = 0; j < SPI_BULK_TEST_SIZE; j++) {
			data_out[j] = rand();
		}

		USBTHING_spi_transfer(usbthing, data_out, data_in, SPI_BULK_TEST_SIZE);

		//Compare sent and received values
		if (strncmp((const char*)data_out, (const char*)data_in, SPI_BULK_TEST_SIZE) != 0) {
			printf("SPI bulk test transfer one data mismatch\r\n");
			printf("out: ");
			for (int i = 0; i < SPI_BULK_TEST_SIZE; i++) {
				printf("%.2x ", data_out[i]);
			}
			printf("\r\n");
			printf("in: ");
			for (int i = 0; i < SPI_BULK_TEST_SIZE; i++) {
				printf("%.2x ", data_in[i]);
			}
			printf("\r\n");
			return -1;
		}

	}

	printf("SPI bulk transfers complete\r\n");

	//TODO: check CS

	//TODO: check CLK

	USBTHING_spi_close(usbthing);

	return 0;
}

static int test_adc(struct usbthing_s* usbthing, int interactive)
{
	float val;

	printf("ADC test\r\n");
	if (interactive != 0) {
		printf("Connect ADC ch 0 to 3V3, ADC ch 3 to GND, and press any key to continue\r\n");
		printf("Note that the ADC GND and 3V3 are incorrectly labeled on the v0.2.0 board\r\n");
		getchar();
	}

	USBTHING_adc_configure(usbthing, USBTHING_ADC_REF_VDD);

	USBTHING_adc_get(usbthing, 0, &val);
	float expected_low = (3.3 * 0.005);
	if (val > expected_low) {
		printf("Error: channel 0 expected: < %.1f actual: %.1f\r\n", expected_low, val);
		return -1;
	}

	USBTHING_adc_get(usbthing, 3, &val);
	float expected_high = (3.3 * 0.995);
	if (val < expected_high) {
		printf("Channel 3 expected: > %.1f actual: %.1f\r\n", expected_high, val);
		return -1;
	}

	return 0;
}

static int test_dac_adc(struct usbthing_s* usbthing, int interactive)
{
	char c;
	float val;
	float out;

	printf("DAC to ADC test\r\n");
	if (interactive != 0) {
		printf("Connect DAC to ADC ch 2 and press any key to continue\r\n");
		getchar();
	}

	USBTHING_adc_configure(usbthing, USBTHING_ADC_REF_VDD);
	USBTHING_dac_configure(usbthing);

	USBTHING_dac_set(usbthing, 1, 3.3);
	usleep(1000);
	USBTHING_adc_get(usbthing, 1, &val);
	if (val < (3.3 * 0.990)) {
		printf("DAC->ADC test error, expected: %.4f got: %.4f\r\n", 3.3, val);
		return -1;
	}

	USBTHING_dac_set(usbthing, 1, 0.0);
	usleep(1000);
	USBTHING_adc_get(usbthing, 1, &val);
	if (val > (3.3 * 0.010)) {
		printf("DAC->ADC test error, expected: %.4f got: %.4f\r\n", 0.0, val);
		return -1;
	}

	printf("DAC to ADC test complete\r\n");

	return 0;
}

static int test_i2c(struct usbthing_s* usbthing, int interactive)
{

	//I don't even know how to start this

	return 0;
}


