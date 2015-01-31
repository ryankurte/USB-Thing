
#ifndef PLATFORM_H
#define PLATFORM_H

/***			STATUS LED Pins 		***/
#define LED_CONN_PIN		
#define LED_CONN_PORT		
#define LED_WAKE_PIN		
#define LED_WAKE_PORT		
#define LED_BUSY_PIN		
#define LED_BUSY_PORT		

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

/*** 			SPI Pins 				***/
#define SPI_MOSI_PIN 		10
#define SPI_MOSI_PORT	 	gpioPortE
#define SPI_MISO_PIN 		11
#define SPI_MISO_PORT	 	gpioPortE
#define SPI_CLK_PIN 		12
#define SPI_CLK_PORT 		gpioPortE
#define SPI_CS_PIN 			13
#define SPI_CS_PORT 		gpioPortE

/*** 			I2C Pins 				***/
#define I2C0_SDA_PIN 		6
#define I2C0_SDA_PORT	 	gpioPortD
#define I2C0_SCL_PIN 		7
#define I2C0_SCL_PORT	 	gpioPortD

/*** 			PWM Pins 				***/
#define TIM2_CC0_PIN 		8
#define TIM2_CC0_PORT	 	gpioPortC
#define TIM2_CC1_PIN 		9
#define TIM2_CC1_PORT	 	gpioPortC

/*** 			ADC Pins 				***/
//ADC0_CH0
#define GPIO0_PIN 			0
#define GPIO0_PORT 			gpioPortD
//ADC0_CH1
#define GPIO1_PIN 			1
#define GPIO1_PORT 			gpioPortD
//ADC0_CH2
#define GPIO2_PIN 			2
#define GPIO2_PORT 			gpioPortD
//ADC0_CH3
#define GPIO3_PIN 			3
#define GPIO3_PORT 			gpioPortD

/*** 			DAC Pins				***/
//DAC0_OUT0
#define DAC0_PIN 			1
#define DAC0_PORT  	 		gpioPortB

/*** 			CLK Pins 				***/
//CMU_CLK0
#define GPIO0_PIN 			1
#define GPIO0_PORT 			gpioPortA
//CMU_CLK1
#define GPIO1_PIN 			2
#define GPIO1_PORT 			gpioPortA


#endif
