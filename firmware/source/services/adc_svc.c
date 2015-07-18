//ADC USB protocol to peripheral mapping

#include "services/adc_svc.h"

#include <stdint.h>

#include "em_usb.h"
#include "em_adc.h"

#include "callbacks.h"
#include "protocol.h"
#include "peripherals/adc.h"

static int adc_config(const USB_Setup_TypeDef *setup);
static int adc_config_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining);
static int adc_get(const USB_Setup_TypeDef *setup);

extern uint8_t cmd_buffer[];
static uint8_t adc_configured = 0;

int adc_handle_setup(const USB_Setup_TypeDef *setup)
{
	switch (setup->wValue) {
	case USBTHING_ADC_CMD_CONFIG:
		return adc_config(setup);
	case USBTHING_ADC_CMD_GET:
		return adc_get(setup);
	}

	return USB_STATUS_REQ_UNHANDLED;
}

static int adc_config(const USB_Setup_TypeDef *setup)
{
	int res = USB_STATUS_REQ_ERR;

	CHECK_SETUP_OUT(USBTHING_CMD_ADC_CONFIG_SIZE);

	res = USBD_Read(0, cmd_buffer, USBTHING_CMD_ADC_CONFIG_SIZE, adc_config_cb);

	return res;
}

static int adc_config_cb(USB_Status_TypeDef status, uint32_t xferred, uint32_t remaining)
{
	(void)xferred;
	(void)remaining;

	struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

	int ref;
	switch (ctrl->adc_cmd.config.ref) {
	case USBTHING_ADC_REF_1V25:
		ref = adcRef1V25;
		break;
	case USBTHING_ADC_REF_2V5:
		ref = adcRef2V5;
		break;
	case USBTHING_ADC_REF_VDD:
		ref = adcRefVDD;
		break;
	}

	//Initialize I2C
	ADC_init(ref);

	adc_configured = 1;

	return USB_STATUS_OK;
}

static int adc_get(const USB_Setup_TypeDef *setup)
{
	int res = USB_STATUS_REQ_ERR;

	CHECK_SETUP_IN(USBTHING_CMD_ADC_GET_SIZE);

	struct usbthing_ctrl_s *ctrl = (struct usbthing_ctrl_s*)&cmd_buffer;

	if (adc_configured == 0) {
		return USB_STATUS_DEVICE_UNCONFIGURED;
	}

	int channel;
	//TODO: labels are backwards, this is a bit silly.
	switch (setup->wIndex) {
	case USBTHING_ADC_CH0:
		channel = adcSingleInpCh3;
		break;
	case USBTHING_ADC_CH1:
		channel = adcSingleInpCh2;
		break;
	case USBTHING_ADC_CH2:
		channel = adcSingleInpCh1;
		break;
	case USBTHING_ADC_CH3:
		channel = adcSingleInpCh0;
		break;
	}

	ctrl->adc_cmd.get.value = ADC_get(channel);
	res = USBD_Write(0, cmd_buffer, USBTHING_CMD_ADC_GET_SIZE, NULL);
	return res;
}
