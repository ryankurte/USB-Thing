//SPI USB protocol to peripheral mapping

#include "services/spi_svc.h"

#include <stdint.h>

#include "em_usb.h"

#include "callbacks.h"
#include "protocol.h"
#include "peripherals/spi.h"
#include "em_usart.h"

#define SPI_BUFF_SIZE 		256

static int spi_config(const USB_Setup_TypeDef *setup);
static int spi_config_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);

//Aligned buffers for USB operations
STATIC_UBUF(spi_receive_buffer, SPI_BUFF_SIZE);
STATIC_UBUF(spi_transmit_buffer, SPI_BUFF_SIZE);

extern uint8_t cmd_buffer[];
static int spi_configured = 0;

int spi_handle_setup(const USB_Setup_TypeDef *setup)
{
	switch (setup->wValue) {
	case USBTHING_SPI_CMD_CONFIG:
		return spi_config(setup);
	}

	return USB_STATUS_REQ_UNHANDLED;
}

static int spi_config(const USB_Setup_TypeDef *setup)
{
	int res = USB_STATUS_REQ_ERR;

	CHECK_SETUP_OUT(USBTHING_CMD_SPI_CONFIG_SIZE);

	res = USBD_Read(0, cmd_buffer, USBTHING_CMD_SPI_CONFIG_SIZE, spi_config_cb);

	return res;
}

static int spi_config_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	(void)xferred;
	(void)remaining;

	struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

	uint8_t freq_le = ctrl->spi_cmd.config.freq_le;
	uint8_t clock_mode = ctrl->spi_cmd.config.clk_mode;

	//Initialize I2C
	SPI_init(freq_le, clock_mode);

	spi_configured = 1;

	return USB_STATUS_OK;
}

void spi_start()
{
	USBD_Read(EP1_OUT, spi_receive_buffer, SPI_BUFF_SIZE, spi_data_receive_cb);
}

int spi_data_sent_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	/* Remove warnings for unused variables */
	(void)xferred;
	(void)remaining;

	//Restart EP_OUT
	USBD_Read(EP1_OUT, spi_receive_buffer, SPI_BUFF_SIZE, spi_data_receive_cb);

	if ( status != USB_STATUS_OK ) {
		/* Handle error */
	}
	return USB_STATUS_OK;
}

int spi_data_receive_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	/* Remove warnings for unused variables */
	(void)xferred;
	(void)remaining;

	//TODO: what if SPI is not initialized?

	/* Check status to verify that the transfer has completed successfully */
	if ( status == USB_STATUS_OK ) {
		SPI_transfer(xferred, spi_receive_buffer, spi_transmit_buffer);
		USBD_Write(EP1_IN, spi_transmit_buffer, xferred, spi_data_receive_cb);

	} else {
		//TODO: handle errors

		//Restart EP_OUT
		USBD_Read(EP1_OUT, spi_receive_buffer, SPI_BUFF_SIZE, spi_data_receive_cb);
	}

	return USB_STATUS_OK;
}