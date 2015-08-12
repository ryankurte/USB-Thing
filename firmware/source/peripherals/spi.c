
#include "peripherals/spi.h"

#include <stdint.h>
#include <stdbool.h>

#include "em_usart.h"
#include "em_gpio.h"
#include "em_cmu.h"

#include "platform.h"

int8_t SPI_init(uint32_t baud, uint8_t clock_mode)
{
    //Enable clocks
    CMU_ClockEnable(SPI_CLOCK, true);
    CMU_ClockEnable(GPIO_CLOCK, true);

    //Set up pins
    GPIO_PinModeSet(SPI_MOSI_PORT,  SPI_MOSI_PIN,  gpioModePushPull, 1);
    GPIO_PinModeSet(SPI_MISO_PORT,  SPI_MISO_PIN,  gpioModeInput,    0);
    GPIO_PinModeSet(SPI_CS_PORT,  SPI_CS_PIN,  gpioModePushPull, 1);
    GPIO_PinModeSet(SPI_CLK_PORT, SPI_CLK_PIN, gpioModePushPull, 1);

    GPIO_PinOutSet(SPI_CS_PORT, SPI_CS_PIN);

    //TODO: determine clock mode

    //Configure USART
    USART_InitSync_TypeDef spiConfig = {
        .enable = usartEnable,
        .refFreq = 0,
        .baudrate = baud,
        .databits = usartDatabits8,
        .master = true,
        .msbf = true,
        .clockMode = clock_mode,
    };

    //Initialize USART
    USART_InitSync(SPI_DEVICE, &spiConfig);

    //Set up route
    SPI_DEVICE->ROUTE |= SPI_ROUTE;

    return 0;
}

int8_t SPI_close()
{
    GPIO_PinModeSet(SPI_MOSI_PORT,  SPI_MOSI_PIN,  gpioModeDisabled, 1);
    GPIO_PinModeSet(SPI_MISO_PORT,  SPI_MISO_PIN,  gpioModeDisabled,    0);
    GPIO_PinModeSet(SPI_CS_PORT,  SPI_CS_PIN,  gpioModeDisabled, 1);
    GPIO_PinModeSet(SPI_CLK_PORT, SPI_CLK_PIN, gpioModeDisabled, 1);

    CMU_ClockEnable(SPI_CLOCK, false);
    CMU_ClockEnable(GPIO_CLOCK, false);
}

int8_t SPI_transfer(uint8_t length, uint8_t *data_out, uint8_t *data_in)
{
    //Enable USART
    USART_Enable(SPI_DEVICE, usartEnable);

    //Assert CS
    GPIO_PinOutClear(SPI_CS_PORT, SPI_CS_PIN);

    //Transfer data
    for (uint8_t i = 0; i < length; i++) {
        data_in[i] = USART_SpiTransfer(SPI_DEVICE, data_out[i]);
    }

    //De-assert CS
    GPIO_PinOutSet(SPI_CS_PORT, SPI_CS_PIN);

    //Disable USART
    USART_Enable(SPI_DEVICE, usartDisable);
    return 0;
}