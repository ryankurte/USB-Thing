//SPI USB protocol to peripheral mapping

#include "services/spi_svc.h"

#include <stdint.h>

#include "em_usb.h"

#include "handlers.h"
#include "protocol.h"
#include "peripherals/spi.h"
#include "em_usart.h"

#define SPI_BUFF_SIZE 		512

static int spi_svc_config(const USB_Setup_TypeDef *setup);
static int spi_svc_config_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
static int spi_svc_data_sent_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
static int spi_svc_data_receive_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
static int spi_svc_close(const USB_Setup_TypeDef *setup);


//Aligned buffers for USB operations
STATIC_UBUF(spi_svc_receive_buffer, SPI_BUFF_SIZE);
STATIC_UBUF(spi_svc_transmit_buffer, SPI_BUFF_SIZE);

extern uint8_t cmd_buffer[];
extern int usbthing_busy;

static int spi_svc_configured = 0;

void spi_svc_start()
{
	//Start listening on SPI endpoint
	USBD_Read(EP1_OUT, spi_svc_receive_buffer, SPI_BUFF_SIZE, spi_svc_data_receive_cb);
}

int spi_svc_handle_setup(const USB_Setup_TypeDef *setup)
{
	switch (setup->wValue) {
	case USBTHING_SPI_CMD_CONFIG:
		return spi_svc_config(setup);
	case USBTHING_SPI_CMD_CLOSE:
		return spi_svc_close(setup);
	}

	return USB_STATUS_REQ_UNHANDLED;
}

static int spi_svc_config(const USB_Setup_TypeDef *setup)
{
	int res = USB_STATUS_REQ_ERR;

	CHECK_SETUP_OUT(USBTHING_CMD_SPI_CONFIG_SIZE);

	res = USBD_Read(0, cmd_buffer, USBTHING_CMD_SPI_CONFIG_SIZE, spi_svc_config_cb);

	return res;
}

static int spi_svc_close(const USB_Setup_TypeDef *setup)
{
	int res = USB_STATUS_OK;

	SPI_close();

	return res;
}

static int spi_svc_config_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	(void)xferred;
	(void)remaining;

	struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

	uint32_t freq_le = ctrl->spi_cmd.config.freq_le;
	uint8_t clock_mode = ctrl->spi_cmd.config.clk_mode;

	//Initialize I2C
	SPI_init(freq_le, clock_mode);

	spi_svc_configured = 1;

	return USB_STATUS_OK;
}

static int spi_svc_data_sent_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	int res;

	/* Remove warnings for unused variables */
	(void)xferred;
	(void)remaining;

	if ( status != USB_STATUS_OK ) {
		/* Handle error */
		return status;
	}

	//if ((xferred != 0) && (xferred % USB_MAX_EP_SIZE == 0)) {
	//	//Send zero length packet to signify completion
	//	res = USBD_Write(EP1_IN, NULL, 0, spi_svc_data_sent_cb);
	//}

	//Restart EP_OUT
	res = USBD_Read(EP1_OUT, spi_svc_receive_buffer, SPI_BUFF_SIZE, spi_svc_data_receive_cb);

	return res;
}

static int spi_svc_data_receive_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	int res;

	/* Remove warnings for unused variables */
	(void)xferred;
	(void)remaining;

	/* Check status to verify that the transfer has completed successfully */
	if ( status != USB_STATUS_OK ) {
		//TODO: handle errors
		return status;
	}

	//Ensure SPI is initialized
	if (spi_svc_configured == 0) {
		return USB_STATUS_DEVICE_UNCONFIGURED;
	}

	usbthing_busy = 1;

	//Check whether a zero length termination packet is required
	//if ((xferred != 0) && (xferred % USB_MAX_EP_SIZE == 0)) {
	//	res = USBD_Read(EP1_OUT, NULL, 0, NULL);
	//}

	for(int i=0; i<xferred; i++) {
		spi_svc_transmit_buffer[i] = 0x00;
	}

	//Perform SPI transfer
	SPI_transfer(xferred, spi_svc_receive_buffer, spi_svc_transmit_buffer);

	//Write result back to host
	res = USBD_Write(EP1_IN, spi_svc_transmit_buffer, xferred, spi_svc_data_sent_cb);

	return res;
}

