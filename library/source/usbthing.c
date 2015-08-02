/**
 * @brief USB Thing Interface Library
 * @details [long description]
 *
 * see: http://libusb.sourceforge.net/api-1.0
 */

#include "usbthing.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "libusb-1.0/libusb.h"

#include "protocol.h"

#define CONTROL_REQUEST_TYPE_IN  (LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE)
#define CONTROL_REQUEST_TYPE_OUT  (LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE)
#define USBTHING_TIMEOUT        0       //Zero for debug purposes (no timeout)
#define USBTHING_BUFFER_SIZE    64

#define DEBUG_USBTHING

#ifdef DEBUG_USBTHING
#define USBTHING_DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define USBTHING_DEBUG_PRINT(...)
#endif

static void print_buffer(uint8_t length, uint8_t *buffer);
static void print_devs(libusb_device **devs, uint16_t vid_filter, uint16_t pid_filter);



int USBTHING_init()
{
    int res;

    res = libusb_init(NULL);
    if (res < 0) {
        return res;
    }

    return 0;
}

void USBTHING_close()
{
    libusb_exit(NULL);
}

int USBTHING_list_devices(uint16_t vid_filter, uint16_t pid_filter)
{
    libusb_device **devs;
    ssize_t cnt;

    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0) {
        return (int) cnt;
    }

    print_devs(devs, vid_filter, pid_filter);

    libusb_free_device_list(devs, 1);

    return (int)cnt;
}

int USBTHING_connect(struct usbthing_s *usbthing, uint16_t vid_filter, uint16_t pid_filter)
{
    int res;

    //Connect to device
    usbthing->handle = libusb_open_device_with_vid_pid(NULL, vid_filter, pid_filter);

    if (usbthing->handle == NULL) {
        //Device not found (or error)
        return -1;
    }

    //Claim appropriate interface
    res = libusb_claim_interface(usbthing->handle, 0);
    if (res != 0) {
        //Could not claim interface
        return -2;
    }

    USBTHING_DEBUG_PRINT("Connected to device: %.4x:%.4x\r\n", vid_filter, pid_filter);

    //Connected
    return 0;
}

int USBTHING_disconnect(struct usbthing_s *usbthing)
{
    //Check device is open
    if (usbthing->handle == NULL) {
        return -1;
    }

    libusb_close(usbthing->handle);

    usbthing->handle = NULL;

    return 0;
}

static int control_set(struct usbthing_s *usbthing, uint32_t service, uint32_t operation, uint32_t index, uint8_t size, uint8_t* data) {
    int res;

    int response_length;

    USBTHING_DEBUG_PRINT("Control send to service: 0x%x operation 0x%x index: 0x%x data: ", service, operation, index);
    for (int i = 0; i < size; i++) {
        USBTHING_DEBUG_PRINT("%.2x ", data[i]);
    }
    USBTHING_DEBUG_PRINT("\r\n");

    res = libusb_control_transfer (usbthing->handle,
                                   CONTROL_REQUEST_TYPE_OUT,
                                   service,
                                   operation,
                                   index,       // Service device index (default to zero)
                                   data,        // Data to be transferred
                                   size,        // Size of data to be transferred
                                   USBTHING_TIMEOUT);

    USBTHING_DEBUG_PRINT("Control send complete\r\n");

    return res;
}

static int control_get(struct usbthing_s *usbthing, uint32_t service, uint32_t operation, uint32_t index, uint8_t size, uint8_t* data) {
    int res;

    int response_length;

    USBTHING_DEBUG_PRINT("Started control fetch from service: 0x%x\r\n", service);

    res = libusb_control_transfer (usbthing->handle,
                                   CONTROL_REQUEST_TYPE_IN,
                                   service,
                                   operation,
                                   index,       // Service device index (default to zero)
                                   data,        // Data to be transferred
                                   size,        // Size of data to be transferred
                                   USBTHING_TIMEOUT);

    USBTHING_DEBUG_PRINT("Control fetch from service: 0x%x operation 0x%x index: 0x%x complete, data: ", service, operation, index);
    for (int i = 0; i < size; i++) {
        USBTHING_DEBUG_PRINT("%.2x ", data[i]);
    }
    USBTHING_DEBUG_PRINT("\r\n");

    return res;
}

int USBTHING_get_firmware_version(struct usbthing_s *usbthing, int length, char *version)
{
    int res;

    struct usbthing_ctrl_s cmd;

    res = control_get(usbthing,
                      USBTHING_MODULE_BASE,
                      BASE_CMD_FIRMWARE_GET,
                      0,
                      USBTHING_CMD_FIRMWARE_GET_SIZE,
                      cmd.data);

    if (res >= 0) {
        USBTHING_DEBUG_PRINT("firmware: %s\n", cmd.base_cmd.firmware_get.version);
        strncpy(version, (const char*)cmd.base_cmd.firmware_get.version, length);
        version[length] = '\0';
    }

    return res;
}

int USBTHING_led_set(struct usbthing_s *usbthing, int led, int enable)
{
    int res;

    struct usbthing_ctrl_s cmd;

    cmd.base_cmd.led_set.pin = led;
    cmd.base_cmd.led_set.enable = enable;

    res = control_set(usbthing,
                      USBTHING_MODULE_BASE,
                      BASE_CMD_LED_SET,
                      0,
                      USBTHING_CMD_LED_SET_SIZE,
                      cmd.data);

    return res;
}

int USBTHING_gpio_configure(struct usbthing_s *usbthing, int pin, int output, int pull_enabled, int pull_up)
{
    int res;
    struct usbthing_ctrl_s cmd;

    //TODO: Sanity check mode and pin inputs?
    cmd.gpio_cmd.config.pin = pin;
    cmd.gpio_cmd.config.mode = output;
    if (pull_enabled == 0) {
        cmd.gpio_cmd.config.pull = 0;
    } else {
        cmd.gpio_cmd.config.pull = (pull_up == 0) ? USBTHING_GPIO_PULL_LOW : USBTHING_GPIO_PULL_HIGH;
    }
    cmd.gpio_cmd.config.interrupt = 0;

    res = control_set(usbthing,
                      USBTHING_MODULE_GPIO,
                      USBTHING_GPIO_CMD_CONFIG,
                      0,
                      USBTHING_CMD_GPIO_CFG_SIZE,
                      cmd.data);

    return res;
}


int USBTHING_gpio_set(struct usbthing_s *usbthing, int pin, int value)
{
    int res;
    struct usbthing_ctrl_s cmd;

    //TODO: Sanity check mode and pin inputs?
    cmd.gpio_cmd.set.pin = pin;
    cmd.gpio_cmd.set.level = value;

    res = control_set(usbthing,
                      USBTHING_MODULE_GPIO,
                      USBTHING_GPIO_CMD_SET,
                      0,
                      USBTHING_CMD_GPIO_SET_SIZE,
                      cmd.data);

    return res;
}

int USBTHING_gpio_get(struct usbthing_s *usbthing, int pin, int *value)
{
    int res;
    struct usbthing_ctrl_s cmd;

    //TODO: Sanity check mode and pin inputs?

    res = control_get(usbthing,
                      USBTHING_MODULE_GPIO,
                      USBTHING_GPIO_CMD_GET,
                      pin,
                      USBTHING_CMD_GPIO_GET_SIZE,
                      cmd.data);

    if (res >= 0) {
        (*value) = (cmd.gpio_cmd.get.level == 0) ? 0 : 1;
    }

    return res;
}


int USBTHING_gpio_get_int(struct usbthing_s *usbthing, int pin, int *value)
{
    return -1;
}

int USBTHING_pwm_configure(struct usbthing_s *usbthing, unsigned int frequency)
{
    return -1;
}

int USBTHING_pwm_enable(struct usbthing_s *usbthing, int channel, int enable)
{
    return -1;
}

int USBTHING_pwm_set(struct usbthing_s *usbthing, int channel, int duty_cycle)
{
    return -1;
}

int USBTHING_dac_configure(struct usbthing_s *usbthing)
{
    return -1;
}

int USBTHING_dac_enable(struct usbthing_s *usbthing, int enable)
{
    return -1;
}

int USBTHING_dac_set(struct usbthing_s *usbthing, unsigned int value)
{
    return -1;
}

int USBTHING_adc_configure(struct usbthing_s *usbthing, unsigned int reference)
{
    int res;

    struct usbthing_ctrl_s ctrl;

    ctrl.adc_cmd.config.ref = reference;

    res = control_set(usbthing,
                      USBTHING_MODULE_ADC,
                      USBTHING_ADC_CMD_CONFIG,
                      0,
                      USBTHING_CMD_ADC_CONFIG_SIZE,
                      ctrl.data);

    return res;
}

int USBTHING_adc_get(struct usbthing_s *usbthing, int channel, unsigned int *value)
{
    int res;

    struct usbthing_ctrl_s ctrl;

    res = control_get(usbthing,
                      USBTHING_MODULE_ADC,
                      USBTHING_ADC_CMD_GET,
                      channel,
                      USBTHING_CMD_ADC_GET_SIZE,
                      ctrl.data);

    *value = ctrl.adc_cmd.get.value;

    return res;
}


int USBTHING_spi_configure(struct usbthing_s *usbthing, unsigned int speed, int mode)
{
    int res;

    struct usbthing_ctrl_s ctrl;

    ctrl.spi_cmd.config.freq_le = speed;
    ctrl.spi_cmd.config.clk_mode = mode;

    res = control_set(usbthing,
                      USBTHING_MODULE_SPI,
                      USBTHING_SPI_CMD_CONFIG,
                      0,
                      USBTHING_CMD_SPI_CONFIG_SIZE,
                      ctrl.data);

    return res;
}

int USBTHING_spi_transfer(struct usbthing_s *usbthing, unsigned char *data_out, unsigned char *data_in, int length)
{
    int res;
    int transferred;

    libusb_clear_halt(usbthing->handle, 0x01);

    USBTHING_DEBUG_PRINT("SPI write: ");
    print_buffer(length, data_out);
    USBTHING_DEBUG_PRINT("\r\n");

    res = libusb_bulk_transfer (usbthing->handle,
                                0x01,
                                data_out,
                                length,
                                &transferred,
                                USBTHING_TIMEOUT);

    //TODO: check for complete transfer
    if (res < 0) {
        perror("USBTHING spi transfer outgoing error");
        return -1;
    }

    //Check if ZLP is required to signify end of transfer
    //(Required if length is multiple of endpoint size)
    if (length % 64 == 0) {
        res = libusb_bulk_transfer (usbthing->handle,
                                    0x01,
                                    NULL,
                                    0,
                                    &transferred,
                                    USBTHING_TIMEOUT);
    }

    USBTHING_DEBUG_PRINT("SPI write complete\r\n");
    USBTHING_DEBUG_PRINT("SPI read\r\n");

    res = libusb_bulk_transfer (usbthing->handle,
                                0x81,
                                data_in,
                                length,
                                &transferred,
                                USBTHING_TIMEOUT);

    USBTHING_DEBUG_PRINT("Transferred: %d\r\n", transferred);

    //TODO: check for complete transfer
    if (res < 0) {
        perror("USBTHING spi transfer incoming error");
        return -2;
    }

    USBTHING_DEBUG_PRINT("SPI read complete: ");
    print_buffer(length, data_in);
    USBTHING_DEBUG_PRINT("\r\n");

    return 0;
}

int USBTHING_i2c_configure(struct usbthing_s *usbthing, int speed)
{
    int res;
    res = libusb_control_transfer (usbthing->handle,
                                   LIBUSB_REQUEST_TYPE_VENDOR,
                                   USBTHING_CMD_I2C_CFG,
                                   speed,
                                   0,
                                   NULL,
                                   USBTHING_I2C_CFG_SIZE,
                                   USBTHING_TIMEOUT);

    if (res < 0) {
        perror("USBTHING i2c configuration error");
    }

    return res;
}

int USBTHING_i2c_write(struct usbthing_s *usbthing,
                       int address,
                       int length_out, unsigned char *data_out)
{
    uint8_t output_buffer[USBTHING_BUFFER_SIZE];
    uint8_t input_buffer[USBTHING_BUFFER_SIZE];
    int output_buffer_length;
    int res;
    int transferred;

    struct usbthing_i2c_transfer_s *config;
    config = (struct usbthing_i2c_transfer_s *) output_buffer;

    //Configure transfer
    config->mode = USBTHING_I2C_MODE_WRITE;
    config->address = address;
    config->num_write = length_out;

    //Copy data
    memcpy(output_buffer + sizeof(struct usbthing_i2c_transfer_s), data_out, length_out);
    output_buffer_length = length_out + sizeof(struct usbthing_i2c_transfer_s);

    USBTHING_DEBUG_PRINT("I2C write: ");
    print_buffer(length_out, data_out);

    res = libusb_bulk_transfer (usbthing->handle,
                                0x02,
                                output_buffer,
                                output_buffer_length,
                                &transferred,
                                USBTHING_TIMEOUT);

    //TODO: check for complete transfer
    if (res < 0) {
        perror("USBTHING i2c write outgoing error");
        return -1;
    }

    if (length_out % 64 == 0) {
        res = libusb_bulk_transfer (usbthing->handle,
                                    0x02,
                                    NULL,
                                    0,
                                    &transferred,
                                    USBTHING_TIMEOUT);
    }

    printf("I2C write complete\r\n");

    //Stub for write function response (written data)
    res = libusb_bulk_transfer (usbthing->handle,
                                0x82,
                                input_buffer,
                                sizeof(input_buffer),
                                &transferred,
                                USBTHING_TIMEOUT);

    //TODO: check for complete write
    if (res < 0) {
        perror("USBTHING i2c write incoming error");
        return -2;
    }

    return 0;
}

int USBTHING_i2c_read(struct usbthing_s *usbthing,
                      int address,
                      int length_in, unsigned char *data_in)
{
    uint8_t output_buffer[USBTHING_BUFFER_SIZE];
    int output_buffer_length;
    int res;
    int transferred;

    struct usbthing_i2c_transfer_s *config;
    config = (struct usbthing_i2c_transfer_s *) output_buffer;

    //Configure transfer
    config->mode = USBTHING_I2C_MODE_READ;
    config->address = address;
    config->num_read = length_in;

    output_buffer_length = sizeof(struct usbthing_i2c_transfer_s);

    res = libusb_bulk_transfer (usbthing->handle,
                                0x02,
                                output_buffer,
                                output_buffer_length,
                                &transferred,
                                USBTHING_TIMEOUT);

    //TODO: check for complete transfer
    if (res < 0) {
        perror("USBTHING i2c read outgoing error");
        return -1;
    }

    if (output_buffer_length % 64 == 0) {
        res = libusb_bulk_transfer (usbthing->handle,
                                    0x02,
                                    NULL,
                                    0,
                                    &transferred,
                                    USBTHING_TIMEOUT);
    }


    res = libusb_bulk_transfer (usbthing->handle,
                                0x82,
                                data_in,
                                length_in,
                                &transferred,
                                USBTHING_TIMEOUT);

    //TODO: check for complete transfer
    if (res < 0) {
        perror("USBTHING i2c read incoming error");
        return -2;
    }

    USBTHING_DEBUG_PRINT("I2C Read: ");
    print_buffer(transferred, data_in);

    return 0;
}

int USBTHING_i2c_write_read(struct usbthing_s *usbthing,
                            int address,
                            int length_out, unsigned char *data_out,
                            int length_in, unsigned char *data_in)
{
    uint8_t output_buffer[USBTHING_BUFFER_SIZE];
    int output_buffer_length;
    int res;
    int transferred;

    struct usbthing_i2c_transfer_s *config;
    config = (struct usbthing_i2c_transfer_s *) output_buffer;

    //Configure transfer
    config->mode = USBTHING_I2C_MODE_WRITE_READ;
    config->address = address;
    config->num_write = length_out;
    config->num_read = length_in;

    //Copy data
    memcpy(output_buffer + sizeof(struct usbthing_i2c_transfer_s), data_out, length_out);
    output_buffer_length = length_out + sizeof(struct usbthing_i2c_transfer_s);

    res = libusb_bulk_transfer (usbthing->handle,
                                0x02,
                                output_buffer,
                                output_buffer_length,
                                &transferred,
                                USBTHING_TIMEOUT);

    //TODO: check for complete transfer
    if (res < 0) {
        perror("USBTHING i2c read write outgoing error");
        return -1;
    }

    if (length_out % 64 == 0) {
        res = libusb_bulk_transfer (usbthing->handle,
                                    0x02,
                                    NULL,
                                    0,
                                    &transferred,
                                    USBTHING_TIMEOUT);
    }


    USBTHING_DEBUG_PRINT("I2C write: ");
    print_buffer(length_out, data_out);

    res = libusb_bulk_transfer (usbthing->handle,
                                0x82,
                                data_in,
                                length_in,
                                &transferred,
                                USBTHING_TIMEOUT);

    //TODO: check for complete transfer
    if (res < 0) {
        perror("USBTHING i2c read write incoming error");
        return -2;
    }

    USBTHING_DEBUG_PRINT("I2C Read: ");
    print_buffer(transferred, data_in);

    return 0;
}

static void print_buffer(uint8_t length, uint8_t *buffer)
{
    for (uint8_t i = 0; i < length; i++) {
        USBTHING_DEBUG_PRINT("%.2x ", buffer[i]);
    }
    USBTHING_DEBUG_PRINT("\r\n");
}

static void print_devs(libusb_device **devs, uint16_t vid_filter, uint16_t pid_filter)
{
    libusb_device *dev;
    int i = 0, j = 0;
    uint8_t path[8];

    USBTHING_DEBUG_PRINT("Devices:\n");

    while ((dev = devs[i++]) != NULL) {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            perror("failed to get device descriptor");
            return;
        }

        if (((vid_filter == 0) || (vid_filter == desc.idVendor))
                && ((pid_filter == 0) || (pid_filter == desc.idVendor))) {

            USBTHING_DEBUG_PRINT("%04x:%04x (bus %d, device %d)",
                                 desc.idVendor, desc.idProduct,
                                 libusb_get_bus_number(dev), libusb_get_device_address(dev));

            r = libusb_get_port_numbers(dev, path, sizeof(path));
            if (r > 0) {
                USBTHING_DEBUG_PRINT(" path: %d", path[0]);
                for (j = 1; j < r; j++)
                    USBTHING_DEBUG_PRINT(".%d", path[j]);
            }
            USBTHING_DEBUG_PRINT("\n");

        }
    }
}