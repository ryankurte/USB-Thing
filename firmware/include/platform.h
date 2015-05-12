
#ifndef PLATFORM_H
#define PLATFORM_H

/***			STATUS LED Pins 		***/
#define LED_CONN_PIN		8
#define LED_CONN_PORT		gpioPortE
#define LED_ACT_PIN			9
#define LED_ACT_PORT		gpioPortE

/*** 			USER LED Pins 			***/
#define LED0_PIN  		  	4
#define LED0_PORT 		  	gpioPortD
#define LED1_PIN  		  	5
#define LED1_PORT 		  	gpioPortD

/*** 			GPIO Pins 				***/
#define GPIO0_PIN 			3
#define GPIO0_PORT		 	gpioPortA
#define GPIO1_PIN 			4
#define GPIO1_PORT		 	gpioPortA
#define GPIO2_PIN 			5
#define GPIO2_PORT		 	gpioPortA
#define GPIO3_PIN 			8
#define GPIO3_PORT		 	gpioPortA
#define GPIO4_PIN 			9
#define GPIO4_PORT		 	gpioPortA
#define GPIO5_PIN 			10
#define GPIO5_PORT		 	gpioPortA
#define GPIO6_PIN 			14
#define GPIO6_PORT		 	gpioPortE
#define GPIO7_PIN 			15
#define GPIO7_PORT		 	gpioPortE

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
#define I2C_SDA_PIN 		4
#define I2C_SDA_PORT	 	gpioPortC
#define I2C_SCL_PIN 		5
#define I2C_SCL_PORT	 	gpioPortC

#define I2C_DEVICE 			I2C0
#define I2C_CLOCK 			cmuClock_I2C0
#define I2C_ROUTE 			I2C_ROUTE_LOCATION_LOC1 | I2C_ROUTE_SCLPEN | I2C_ROUTE_SDAPEN

/*** 			PWM Pins 				***/
#define PWM_TIMER_CLOCK		cmuClock_HFPER
#define PWM_TIMER 			TIMER2
#define TIM2_CC0_PIN 		7
#define TIM2_CC0_PORT	 	gpioPortB
#define TIM2_CC1_PIN 		8
#define TIM2_CC1_PORT	 	gpioPortB

/*** 			ADC Pins 				***/
#define ADC_DEVICE			ADC0
#define ADC_CLOCK 			cmuClock_ADC0

//ADC0_CH0
#define ADC_CH0_PIN 		0
#define ADC_CH0_PORT 		gpioPortD
//ADC0_CH1
#define ADC_CH1_PIN 		1
#define ADC_CH1_PORT 		gpioPortD
//ADC0_CH2
#define ADC_CH2_PIN 		2
#define ADC_CH2_PORT 		gpioPortD
//ADC0_CH3
#define ADC_CH3_PIN 		3
#define ADC_CH3_PORT 		gpioPortD

/*** 			DAC Pins				***/
//DAC0_OUT0
#define DAC_DEVICE			DAC0
#define DAC_CLOCK			cmuClock_DAC0
#define DAC_CHANNEL 		0
#define DAC0_PIN 			11
#define DAC0_PORT  	 		gpioPortB

#endif
