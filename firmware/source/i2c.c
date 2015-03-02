
#include "i2c.h"

#include "em_i2c.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_int.h"

#include "platform.h"

#define ENABLE_INTERRUPTS   INT_Enable();
#define DISABLE_INTERRUPTS  INT_Disable();

/***        Internal function prototypes            ***/

static int8_t transfer(I2C_TransferSeq_TypeDef *i2c_transfer_ptr, I2C_TypeDef *i2c_bus_ptr);

/***        Interface Functions                     ***/

//Initialize I2C peripheral
void I2C_init(uint32_t baud)
{

    //Create configuration structure
    I2C_Init_TypeDef i2c_config = {
        .enable = true,
        .master = true,
        .refFreq = 0,
        .freq = baud,
        .clhr = i2cClockHLRAsymetric
    };

    //Enable device
    CMU_ClockEnable(I2C_CLOCK, true);

    //Setup pins
    GPIO_PinModeSet(I2C_SDA_PORT, I2C_SDA_PIN, gpioModeWiredAnd, 0);
    GPIO_PinModeSet(I2C_SCL_PORT, I2C_SCL_PIN, gpioModeWiredAnd, 0);

    I2C_Init(I2C_DEVICE, &i2c_config);

    //Set route
    I2C_DEVICE->ROUTE = I2C_ROUTE;

    //Enable auto ack
    I2C_DEVICE->CTRL |= I2C_CTRL_AUTOACK;
}

//I2C write
int8_t I2C_write(uint8_t address, uint32_t num_bytes, const uint8_t *data_array)
{
    I2C_TransferSeq_TypeDef i2c_transfer = {
        .addr = address << 1,
        .flags = I2C_FLAG_WRITE,
        .buf[0] = { .data = data_array, .len = num_bytes },
    };

    return transfer(&i2c_transfer, I2C_DEVICE);
}

//I2C read
int8_t I2C_read(uint8_t address, uint32_t num_bytes, uint8_t *data_array)
{
    I2C_TransferSeq_TypeDef i2c_transfer = {
        .addr = address << 1,
        .flags = I2C_FLAG_READ,
        .buf[0] = { .data = data_array, .len = num_bytes },
    };

    return transfer(&i2c_transfer, I2C_DEVICE);
}

//I2C Write and read
int8_t I2C_write_read(uint8_t address, uint32_t num_write_bytes, const uint8_t *write_data_array, uint32_t num_read_bytes, uint8_t *read_data_array)
{
    I2C_TransferSeq_TypeDef i2c_transfer = {
        .addr = address << 1,
        .flags = I2C_FLAG_WRITE_READ,
        .buf[0] = { .data = write_data_array, .len = num_write_bytes },
        .buf[1] = { .data = read_data_array, .len = num_read_bytes },
    };

    return transfer(&i2c_transfer, I2C_DEVICE);
}

/***        Internal function implementations       ***/

//Internal transfer function
static int8_t transfer(I2C_TransferSeq_TypeDef *i2c_transfer_ptr, I2C_TypeDef *i2c_bus_ptr)
{
    int8_t result;

    // I2C bus access must be atomic
    DISABLE_INTERRUPTS;

    result = I2C_TransferInit(i2c_bus_ptr, i2c_transfer_ptr);

    while (result == i2cTransferInProgress) {
        result = I2C_Transfer(i2c_bus_ptr);
    }

    // Reenable I2C access
    ENABLE_INTERRUPTS;

    // TODO handle some errors?
    if (result == i2cTransferDone) {
        return 0;
    } else if (result == i2cTransferNack) {
        return -1;
    } else {
        return -2;
    }
}

