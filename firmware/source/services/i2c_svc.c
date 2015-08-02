//i2c USB protocol to peripheral mapping

#include "services/i2c_svc.h"

#include <stdint.h>

#include "em_usb.h"

#include "callbacks.h"
#include "protocol.h"
#include "peripherals/i2c.h"
#include "em_usart.h"

#define i2c_BUFF_SIZE 		256

static int i2c_config(const USB_Setup_TypeDef *setup);
static int i2c_config_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);

//Aligned buffers for USB operations
STATIC_UBUF(i2c_receive_buffer, i2c_BUFF_SIZE);
STATIC_UBUF(i2c_transmit_buffer, i2c_BUFF_SIZE);

extern uint8_t cmd_buffer[];
static int i2c_configured = 0;

int i2c_handle_setup(const USB_Setup_TypeDef *setup)
{
	switch (setup->wValue) {
	case USBTHING_i2c_CMD_CONFIG:
		return i2c_config(setup);
	}

	return USB_STATUS_REQ_UNHANDLED;
}

static int i2c_config(const USB_Setup_TypeDef *setup)
{
	int res = USB_STATUS_REQ_ERR;

	CHECK_SETUP_OUT(USBTHING_CMD_i2c_CONFIG_SIZE);

	res = USBD_Read(0, cmd_buffer, USBTHING_CMD_i2c_CONFIG_SIZE, i2c_config_cb);

	return res;
}

static int i2c_config_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	(void)xferred;
	(void)remaining;

	struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

	uint8_t freq_le = ctrl->i2c_cmd.config.freq_le;
	uint8_t clock_mode = ctrl->i2c_cmd.config.clk_mode;

	//Initialize I2C
	i2c_init(freq_le, clock_mode);

	i2c_configured = 1;

	return USB_STATUS_OK;
}

void i2c_start()
{
	USBD_Read(EP1_OUT, i2c_receive_buffer, i2c_BUFF_SIZE, i2c_data_receive_cb);
}

int i2c_data_sent_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	/* Remove warnings for unused variables */
	(void)xferred;
	(void)remaining;

	//Restart EP_OUT
	USBD_Read(EP1_OUT, i2c_receive_buffer, i2c_BUFF_SIZE, i2c_data_receive_cb);

	if ( status != USB_STATUS_OK ) {
		/* Handle error */
	}
	return USB_STATUS_OK;
}

int i2c_data_receive_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	/* Remove warnings for unused variables */
	(void)xferred;
	(void)remaining;

	//TODO: what if i2c is not initialized?

	/* Check status to verify that the transfer has completed successfully */
	if ( status == USB_STATUS_OK ) {
		if (i2c_configured == 0) {
			return USB_STATUS_DEVICE_UNCONFIGURED;
		} else {
			i2c_transfer(xferred, i2c_receive_buffer, i2c_transmit_buffer);
			USBD_Write(EP1_IN, i2c_transmit_buffer, xferred, i2c_data_receive_cb);
		}

	} else {
		//TODO: handle errors

		//Restart EP_OUT
		USBD_Read(EP1_OUT, i2c_receive_buffer, i2c_BUFF_SIZE, i2c_data_receive_cb);
	}

	return USB_STATUS_OK;
}