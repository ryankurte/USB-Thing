

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

enum usbthing_module_e {
    USBTHING_MODULE_BASE = 1,
    USBTHING_MODULE_GPIO = 2,
    USBTHING_MODULE_SPI = 3,
    USBTHING_MODULE_I2C = 4,
    USBTHING_MODULE_PWM = 5,
    USBTHING_MODULE_ADC = 6,
    USBTHING_MODULE_DAC = 7
};

enum usb_thing_cmd_e {
    USBTHING_CMD_NOP = 0x80,
    USBTHING_CMD_SERIAL_GET = 0x81,
    USBTHING_CMD_FIRMWARE_GET = 0x82,
    USBTHING_CMD_LED_SET = 0x83,
    USBTHING_CMD_RESET = 0x84,
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
    USBTHING_ERROR_PERIPHERAL_TIMEOUT = -4
};


/*****       Protocol Configuration         *****/
#define USBTHING_SERIAL_MAX_SIZE        32
#define USBTHING_FIRMWARE_MAX_SIZE      32
#define USBTHING_SPI_MAX_SIZE           64





/*****       Protocol Configuration         *****/
enum base_cmd_e {
    BASE_CMD_NOOP = 0,
    BASE_CMD_SERIAL_GET = 1,
    BASE_CMD_FIRMWARE_GET = 2,
    BASE_CMD_LED_SET = 3,
    BASE_CMD_RESET = 4
};

struct serial_get_s {
    uint8_t serial[USBTHING_SERIAL_MAX_SIZE];
} __attribute((packed));

struct firmware_get_s {
    uint8_t version[USBTHING_FIRMWARE_MAX_SIZE];
} __attribute((packed));

struct led_set_s {
    uint8_t pin;
    uint8_t enable;
} __attribute((packed));

struct base_cmd_s {
    union {
        struct serial_get_s serial_get;
        struct firmware_get_s firmware_get;
        struct led_set_s led_set;
    };
} __attribute((packed));

#define USBTHING_CMD_NOP_SIZE                   0
#define USBTHING_CMD_SERIAL_GET_SIZE            USBTHING_SERIAL_MAX_SIZE
#define USBTHING_CMD_FIRMWARE_GET_SIZE          USBTHING_FIRMWARE_MAX_SIZE
#define USBTHING_CMD_LED_SET_SIZE               (sizeof(struct led_set_s))

/*****       GPIO Configuration messages        *****/

enum usbthing_gpio_cmd_e {
    USBTHING_GPIO_CMD_CONFIG = 1,
    USBTHING_GPIO_CMD_GET = 2,
    USBTHING_GPIO_CMD_SET = 3
};

enum usbthing_gpio_mode_e {
    USBTHING_GPIO_MODE_INPUT = 0,
    USBTHING_GPIO_MODE_OUTPUT = 1
};

enum usbthing_gpio_level_e {
    USBTHING_GPIO_LEVEL_LOW = 0,
    USBTHING_GPIO_LEVEL_HIGH = 1,
};

enum usbthing_gpio_pull_e {
    USBTHING_GPIO_PULL_NONE = 0,
    USBTHING_GPIO_PULL_LOW = 1,
    USBTHING_GPIO_PULL_HIGH = 2
};

enum usbthing_gpio_int_e {
    USBTHING_GPIO_INT_DISABLE = 0,
    USBTHING_GPIO_INT_RISING = 1,
    USBTHING_GPIO_INT_FALLING = 2
};

struct gpio_config_s {
    uint8_t pin;
    uint8_t mode;
    uint8_t pull;
    uint8_t interrupt;
} gpio_config_s;

struct gpio_set_s {
    uint8_t pin;
    uint8_t level;
} gpio_set_s;

struct gpio_get_s {
    uint8_t level;
} gpio_get_s;

struct gpio_cmd_s {
    union {
        struct gpio_config_s config;
        struct gpio_set_s set;
        struct gpio_get_s get;
    };
} __attribute((packed));

#define USBTHING_CMD_GPIO_CFG_SIZE              (sizeof(gpio_config_s))
#define USBTHING_CMD_GPIO_GET_SIZE              (sizeof(gpio_get_s))
#define USBTHING_CMD_GPIO_SET_SIZE              (sizeof(gpio_set_s))

/*****      ADC Configuration messages          *****/
struct usbthing_adc_config_s {

} usbthing_adc_config_s;

#define ADC_CONFIG_SIZE (sizeof(usbthing_adc_config_s))

/*****      DAC Configuration messages          *****/
struct usbthing_dac_config_s {

} usbthing_dac_config_s;

#define DAC_CONFIG_SIZE (sizeof(usbthing_dac_config_s))

#define USBTHING_CMD_DAC_CFG_SIZE               0
#define USBTHING_CMD_DAC_ENABLE_SIZE            0
#define USBTHING_CMD_DAC_SET_SIZE               4

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
    uint8_t clk_mode;
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
    uint32_t freq_le;                             //!< I2C device speed
} usbthing_i2c_cfg_s;

struct usbthing_i2c_transfer_s {
    uint8_t mode;                               //!< I2C Transfer mode
    uint8_t address;                            //!< I2C Device address
    uint8_t num_write;                          //!< Number of bytes to write
    uint8_t num_read;                           //!< Number of bytes to read
} usbthing_i2c_transfer_s;

struct usbthing_ctrl_s {
    union {
        uint8_t data[32];
        struct base_cmd_s base_cmd;
        struct gpio_cmd_s gpio_cmd;
    };
} usbthing_ctrl_s;

#endif
