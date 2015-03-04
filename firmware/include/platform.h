
#ifndef PLATFORM_H
#define PLATFORM_H

/***			STATUS LED Pins 		***/
#define LED_CONN_PIN		8
#define LED_CONN_PORT		gpioPortE
#define LED_ACT_PIN			9
#define LED_ACT_PORT		gpioPortE

/*** 			USER LED Pins 			***/
#define LED0_PIN  		  	3
#define LED0_PORT 		  	gpioPortE
#define LED1_PIN  		  	2
#define LED1_PORT 		  	gpioPortE

/*** 			GPIO Pins 				***/
#define GPIO0_PIN 			3
#define GPIO0_PORT		 	gpioPortA
#define GPIO1_PIN 			4
#define GPIO1_PORT		 	gpioPortA
#define GPIO2_PIN 			5
#define GPIO2_PORT		 	gpioPortA
#define GPIO3_PIN 			6
#define GPIO3_PORT		 	gpioPortA
#define GPIO4_PIN 			7
#define GPIO4_PORT		 	gpioPortA
#define GPIO5_PIN 			8
#define GPIO5_PORT		 	gpioPortA
#define GPIO6_PIN 			9
#define GPIO6_PORT		 	gpioPortA
#define GPIO7_PIN 			10
#define GPIO7_PORT		 	gpioPortA

#define GPIO_CLOCK 			cmuClock_GPIO

/*** 			SPI Pins 				***/
#define SPI_MOSI_PIN 		10
#define SPI_MOSI_PORT	 	gpioPortE
#define SPI_MISO_PIN 		11
#define SPI_MISO_PORT	 	gpioPortE
#define SPI_CLK_PIN 		12
#define SPI_CLK_PORT 		gpioPortE
#define SPI_CS_PIN 			13
#define SPI_CS_PORT 		gpioPortE

#define SPI_DEVICE 			USART0
#define SPI_CLOCK 			cmuClock_USART0
#define SPI_ROUTE 			USART_ROUTE_LOCATION_LOC0 | USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_CLKPEN | USART_ROUTE_CSPEN

/*** 			I2C Pins 				***/
#define I2C_SDA_PIN 		6
#define I2C_SDA_PORT	 	gpioPortD
#define I2C_SCL_PIN 		7
#define I2C_SCL_PORT	 	gpioPortD

#define I2C_DEVICE 			I2C0
#define I2C_CLOCK 			cmuClock_I2C0
#define I2C_ROUTE 			I2C_ROUTE_LOCATION_LOC1 | I2C_ROUTE_SCLPEN | I2C_ROUTE_SDAPEN

/*** 			PWM Pins 				***/
#define TIM2_CC0_PIN 		8
#define TIM2_CC0_PORT	 	gpioPortC
#define TIM2_CC1_PIN 		9
#define TIM2_CC1_PORT	 	gpioPortC

/*** 			ADC Pins 				***/
//ADC0_CH0
#define ADC0_CH0_PIN 		0
#define ADC0_CH0_PORT 		gpioPortD
//ADC0_CH1
#define ADC0_CH1_PIN 		1
#define ADC0_CH1_PORT 		gpioPortD
//ADC0_CH2
#define ADC0_CH2_PIN 		2
#define ADC0_CH2_PORT 		gpioPortD
//ADC0_CH3
#define ADC0_CH3_PIN 		3
#define ADC0_CH3_PORT 		gpioPortD

/*** 			DAC Pins				***/
//DAC0_OUT0
#define DAC0_PIN 			1
#define DAC0_PORT  	 		gpioPortB

#endif
