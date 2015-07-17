//SPI USB protocol to peripheral mapping

#include "services/spi_svc.h"

#include <stdint.h>

#include "em_usb.h"

#include "callbacks.h"
#include "protocol.h"
#include "peripherals/spi.h"
#include "em_usart.h"

#define SPI_BUFF_SIZE 		256

//Aligned buffers for USB operations
STATIC_UBUF(spi_receive_buffer, SPI_BUFF_SIZE);
STATIC_UBUF(spi_transmit_buffer, SPI_BUFF_SIZE);

static int spi_configured = 0;

void spi_cb_start()
{
	USBD_Read(EP1_OUT, spi_receive_buffer, SPI_BUFF_SIZE, spi_cb_data_receive);
}

int spi_cb_configure(const USB_Setup_TypeDef *setup)
{
	int res = USB_STATUS_REQ_ERR;

	CHECK_SETUP_OUT(USBTHING_SPI_CFG_SIZE);

	uint8_t speed = setup->wValue;
	uint8_t mode = setup->wIndex;
	uint32_t baud, clock_mode;

	//Set baud rate based on speed
	switch (speed) {
	case USBTHING_SPI_SPEED_100KHZ:
		baud = 100000;
		break;
	case USBTHING_SPI_SPEED_400KHZ:
		baud = 400000;
		break;
	case USBTHING_SPI_SPEED_1MHZ:
		baud = 1000000;
		break;
	case USBTHING_SPI_SPEED_5MHZ:
		baud = 5000000;
		break;
	}

	//Set clock mode
	switch (mode) {
	case USBTHING_SPI_CLOCK_MODE0:
		clock_mode = usartClockMode0;
		break;
	case USBTHING_SPI_CLOCK_MODE1:
		clock_mode = usartClockMode1;
		break;
	case USBTHING_SPI_CLOCK_MODE2:
		clock_mode = usartClockMode2;
		break;
	case USBTHING_SPI_CLOCK_MODE3:
		clock_mode = usartClockMode3;
		break;
	}

	//Initialize I2C
	SPI_init(baud, clock_mode);

	spi_configured = 1;

	return USB_STATUS_OK;
}

int spi_cb_data_sent(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	/* Remove warnings for unused variables */
	(void)xferred;
	(void)remaining;

	//Restart EP_OUT
	USBD_Read(EP1_OUT, spi_receive_buffer, SPI_BUFF_SIZE, spi_cb_data_receive);

	if ( status != USB_STATUS_OK ) {
		/* Handle error */
	}
	return USB_STATUS_OK;
}

int spi_cb_data_receive(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	/* Remove warnings for unused variables */
	(void)xferred;
	(void)remaining;

	//TODO: what if SPI is not initialized?

	/* Check status to verify that the transfer has completed successfully */
	if ( status == USB_STATUS_OK ) {
		SPI_transfer(xferred, spi_receive_buffer, spi_transmit_buffer);
		USBD_Write(EP1_IN, spi_transmit_buffer, xferred, spi_cb_data_sent);

	} else {
		//TODO: handle errors

		//Restart EP_OUT
		USBD_Read(EP1_OUT, spi_receive_buffer, SPI_BUFF_SIZE, spi_cb_data_receive);
	}

	return USB_STATUS_OK;
}