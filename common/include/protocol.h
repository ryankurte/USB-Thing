

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define USBTHING_PROTOCOL_VERSION       1

#ifndef VENDOR_ID
#define VENDOR_ID   0x0001
#endif

#ifndef PRODUCT_ID
#define PRODUCT_ID  0x0001
#endif

enum usb_thing_cmd_e {
    USBTHING_CMD_NOP = 0x80,
    USBTHING_CMD_SERIAL_GET = 0x81,
    USBTHING_CMD_FIRMWARE_GET = 0x82,
    USBTHING_CMD_LED_SET = 0x83,
    USBTHING_CMD_GPIO_CFG = 0x91,
    USBTHING_CMD_GPIO_SET = 0x92,
    USBTHING_CMD_GPIO_GET = 0x93,
    USBTHING_CMD_SPI_CFG = 0xA1,
    USBTHING_CMD_I2C_CFG = 0xB1,
    USBTHING_CMD_I2C_TRANSFER = 0xB2,
    USBTHING_CMD_PWM_CFG = 0xC1,
    USBTHING_CMD_PWM_EN = 0xC2,
    USBTHING_CMD_PWM_SET = 0xC3,
    USBTHING_CMD_ADC_CFG = 0xD1,
    USBTHING_CMD_ADC_GET = 0xD2,
    USBTHING_CMD_DAC_CFG = 0xE1,
    USBTHING_CMD_DAC_EN = 0xE2,
    USBTHING_CMD_DAC_SET = 0xE3,
    USBTHING_CMD_UART_CFG = 0xF1
};

enum usb_thing_error_e {
    USBTHING_ERROR_OK = 0,
    USBTHING_ERROR_USB_DISCONNECT = -1,
    USBTHING_ERROR_USB_TIMEOUT = -2,
    USBTHING_ERROR_PERIPHERAL_FAILED = -3,
    USBTHING_ERROR_PERIPHERAL_TIMEOut = -4
};


/*****       Protocol Configuration         *****/
#define USBTHING_SERIAL_MAX_SIZE        32
#define USBTHING_FIRMWARE_MAX_SIZE      32
#define USBTHING_SPI_MAX_SIZE           64


#define USBTHING_CMD_NOP_SIZE                   0
#define USBTHING_CMD_SERIAL_GET_SIZE            USBTHING_SERIAL_MAX_SIZE
#define USBTHING_CMD_FIRMWARE_GET_SIZE          USBTHING_FIRMWARE_MAX_SIZE
#define USBTHING_CMD_LED_SET_SIZE               0

/*****       GPIO Configuration messages        *****/
#define USBTHING_CMD_GPIO_CFG_SIZE              0
//Mode field
#define USBTHING_GPIO_CFG_MODE_SHIFT            (0)
#define USBTHING_GPIO_CFG_MODE_MASK             (1 << USBTHING_GPIO_CFG_MODE_SHIFT)
#define USBTHING_GPIO_CFG_MODE_INPUT            (0 << USBTHING_GPIO_CFG_MODE_SHIFT)
#define USBTHING_GPIO_CFG_MODE_OUTPUT           (1 << USBTHING_GPIO_CFG_MODE_SHIFT)
//Pull enable field
#define USBTHING_GPIO_CFG_PULL_ENABLE_SHIFT     (1)
#define USBTHIGN_GPIO_CFG_PULL_ENABLE_MASK      (1 << USBTHING_GPIO_CFG_PULL_ENABLE_SHIFT)
#define USBTHING_GPIO_CFG_PULL_ENABLE           (0 << USBTHING_GPIO_CFG_PULL_ENABLE_SHIFT)
#define USBTHING_GPIO_CFG_PULL_DISABLE          (1 << USBTHING_GPIO_CFG_PULL_ENABLE_SHIFT)
//Pull direction field
#define USBTHING_GPIO_CFG_PULL_DIRECTION_SHIFT  (2)
#define USBTHIGN_GPIO_CFG_PULL_DIRECTION_MASK   (1 << USBTHING_GPIO_CFG_PULL_DIRECTION_SHIFT)
#define USBTHING_GPIO_CFG_PULL_LOW              (0 << USBTHING_GPIO_CFG_PULL_DIRECTION_SHIFT)
#define USBTHING_GPIO_CFG_PULL_HIGH             (1 << USBTHING_GPIO_CFG_PULL_DIRECTION_SHIFT)
//Interrupt enable field
#define USBTHING_GPIO_CFG_INT_ENABLE_SHIFT      (3)
#define USBTHIGN_GPIO_CFG_INT_ENABLE_MASK       (1 << USBTHING_GPIO_CFG_INT_ENABLE_SHIFT)
#define USBTHING_GPIO_CFG_INT_ENABLE            (0 << USBTHING_GPIO_CFG_INT_ENABLE_SHIFT)
#define USBTHING_GPIO_CFG_INT_DISABLE           (1 << USBTHING_GPIO_CFG_INT_ENABLE_SHIFT)
//Interrupt direction field
#define USBTHING_GPIO_CFG_INT_DIRECTION_SHIFT   (4)
#define USBTHIGN_GPIO_CFG_INT_DIRECTION_MASK    (1 << USBTHING_GPIO_CFG_INT_DIRECTION_SHIFT)
#define USBTHING_GPIO_CFG_INT_RISING            (0 << USBTHING_GPIO_CFG_INT_DIRECTION_SHIFT)
#define USBTHING_GPIO_CFG_INT_FALLING           (1 << USBTHING_GPIO_CFG_INT_DIRECTION_SHIFT)

#define USBTHING_CMD_GPIO_SET_SIZE              0
#define USBTHING_CMD_GPIO_GET_SIZE              1

/*****      ADC Configuration messages          *****/
struct usbthing_adc_config_s {

} usbthing_adc_config_s;

/*****      DAC Configuration messages          *****/
struct usbthing_dac_config_s {

} usbthing_dac_config_s;

/*****      PWM Configuration messages          *****/
struct usbthing_pwm_config_s {
    uint32_t freq_le;
} usbthing_pwm_config_s;

/*****      SPI Configuration messages          *****/
#define USBTHING_SPI_CFG_SPEED_SHIFT            (0)
#define USBTHING_SPI_CFG_SPEED_MASK             (0x0F << USBTHING_I2C_CFG_SPEED_SHIFT)
#define USBTHING_SPI_CFG_CLOCK_SHIFT            (4)
#define USBTHING_SPI_CFG_CLOCK_MASK             (0x03 << USBTHING_SPI_CFG_CPOL_SHIFT)
#define USBTHING_SPI_CFG_SIZE                   0

struct usbthing_spi_cfg_s {
    uint32_t freq_le;
    uint8_t mode;
} usbthing_spi_cfg_s;

enum usbthing_spi_speed_e {
    USBTHING_SPI_SPEED_100KHZ = 0,              //!< Standard mode (100 kbps)
    USBTHING_SPI_SPEED_400KHZ = 1,              //!< Full mode (400 kbps)
    USBTHING_SPI_SPEED_1MHZ = 2,                //!< Fast mode (1 Mpbs)
    USBTHING_SPI_SPEED_5MHZ = 3                 //!< High speed mode (3.2 Mbps)
};

enum usbthing_spi_clock_mode_e {
    USBTHING_SPI_CLOCK_MODE0 = 0,               //!< Idle low, sample leading edge
    USBTHING_SPI_CLOCK_MODE1 = 1,               //!< Idle low, sample trailing edge
    USBTHING_SPI_CLOCK_MODE2 = 2,               //!< Idle high, sample leading edge
    USBTHING_SPI_CLOCK_MODE3 = 3,               //!< Idle low, sample trailing edge
};

/*****      I2C Configuration messages          *****/
#define USBTHING_I2C_CFG_SPEED_SHIFT            (0)
#define USBTHING_I2C_CFG_SPEED_MASK             (0x0F << USBTHING_I2C_CFG_SPEED_SHIFT)
#define USBTHING_I2C_CFG_SIZE                   0

enum usbthing_i2c_speed_e {
    USBTHING_I2C_SPEED_STANDARD = 0,            //!< Standard mode (100 kbps)
    USBTHING_I2C_SPEED_FULL = 1,                //!< Full mode (400 kbps)
    USBTHING_I2C_SPEED_FAST = 2,                //!< Fast mode (1 Mpbs)
    USBTHING_I2C_SPEED_HIGH = 3                 //!< High speed mode (3.2 Mbps)
};

enum usbthing_i2c_transfer_mode_e {
    USBTHING_I2C_MODE_READ = 0,                 //!< Read only mode
    USBTHING_I2C_MODE_WRITE = 1,                //!< Write only mode
    USBTHING_I2C_MODE_WRITE_READ = 2            //!< Write and read mode
};

struct usbthing_i2c_cfg_s {

} usbthing_i2c_cfg_s;

struct usbthing_i2c_transfer_s {
    uint8_t mode;                               //!< I2C Transfer mode
    uint8_t address;                            //!< I2C Device address
    uint8_t num_write;                          //!< Number of bytes to write
    uint8_t num_read;                           //!< Number of bytes to read
} usbthing_i2c_transfer_s __attribute((packed));

#endif
