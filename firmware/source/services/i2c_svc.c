//i2c USB protocol to peripheral mapping

#include "services/i2c_svc.h"

#include <stdint.h>

#include "em_usb.h"

#include "callbacks.h"
#include "protocol.h"
#include "peripherals/i2c.h"
#include "em_usart.h"

#define I2C_BUFF_SIZE 		256

static int i2c_svc_config(const USB_Setup_TypeDef *setup);
static int i2c_svc_config_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);

static int i2c_svc_data_sent_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
static int i2c_svc_data_receive_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
static int i2c_svc_close(const USB_Setup_TypeDef *setup);


//Aligned buffers for USB operations
STATIC_UBUF(i2c_receive_buffer, I2C_BUFF_SIZE);
STATIC_UBUF(i2c_transmit_buffer, I2C_BUFF_SIZE);

extern uint8_t cmd_buffer[];
extern int usbthing_busy;

static int i2c_configured = 0;

void i2c_svc_start()
{
	USBD_Read(EP2_OUT, i2c_receive_buffer, I2C_BUFF_SIZE, i2c_svc_data_receive_cb);
}

int i2c_svc_handle_setup(const USB_Setup_TypeDef *setup)
{
	switch (setup->wValue) {
	case USBTHING_I2C_CMD_CONFIG:
		return i2c_svc_config(setup);
	}

	return USB_STATUS_REQ_UNHANDLED;
}

static int i2c_svc_config(const USB_Setup_TypeDef *setup)
{
	int res = USB_STATUS_REQ_ERR;

	CHECK_SETUP_OUT(USBTHING_CMD_I2C_CONFIG_SIZE);

	res = USBD_Read(0, cmd_buffer, USBTHING_CMD_I2C_CONFIG_SIZE, i2c_svc_config_cb);

	return res;
}

static int i2c_svc_config_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	(void)xferred;
	(void)remaining;

	struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

	uint8_t freq_le = ctrl->i2c_cmd.config.freq_le;
	//uint8_t clock_mode = ctrl->i2c_cmd.config.clk_mode;

	//Initialize I2C
	I2C_init(freq_le);

	i2c_configured = 1;

	return USB_STATUS_OK;
}


int i2c_svc_data_sent_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
    /* Remove warnings for unused variables */
    (void)xferred;
    (void)remaining;

    //Restart EP_OUT
    USBD_Read(EP2_OUT, i2c_receive_buffer, I2C_BUFF_SIZE, i2c_svc_data_receive_cb);

    if ( status != USB_STATUS_OK ) {
        /* Handle error */
    }
    return USB_STATUS_OK;
}

int i2c_svc_data_receive_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
    //Remove unused variable warnings
    (void)xferred;
    (void)remaining;

    //TODO: what if i2c is not initialized?

    if ( status == USB_STATUS_OK ) {
        struct usbthing_i2c_transfer_s *config = (struct usbthing_i2c_transfer_s *) i2c_receive_buffer;

        //TODO: bounds checking of num_write, num_read vs. xferred and maximum

        //Call hardware function based on mode
        switch (config->mode) {
        case USBTHING_I2C_MODE_WRITE:
            I2C_write(config->address, config->num_write, i2c_receive_buffer + sizeof(struct usbthing_i2c_transfer_s));
            //nb. dummy write back to USB (allows ordering/exclusion)
            USBD_Write(EP2_IN, i2c_receive_buffer + sizeof(struct usbthing_i2c_transfer_s), config->num_write, i2c_svc_data_sent_cb);
            break;
        case USBTHING_I2C_MODE_READ:
            I2C_read(config->address, config->num_read, i2c_transmit_buffer);
            USBD_Write(EP2_IN, i2c_transmit_buffer, config->num_read, i2c_svc_data_sent_cb);
            break;
        case USBTHING_I2C_MODE_WRITE_READ:
            I2C_write_read(config->address, config->num_write, i2c_receive_buffer + sizeof(struct usbthing_i2c_transfer_s), config->num_read, i2c_transmit_buffer);
            USBD_Write(EP2_IN, i2c_transmit_buffer, config->num_read, i2c_svc_data_sent_cb);
            break;
        }

    } else {
        //TODO: handle errors

        //Restart EP_OUT
        USBD_Read(EP2_OUT, i2c_receive_buffer, I2C_BUFF_SIZE, i2c_svc_data_receive_cb);
    }

    return USB_STATUS_OK;
}


