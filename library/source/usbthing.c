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

#define CONTROL_REQUEST_TYPE_IN  (LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE)
#define CONTROL_REQUEST_TYPE_OUT  (LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE)
#define USBTHING_TIMEOUT        1000
#define USBTHING_BUFFER_SIZE    64

#include "protocol.h"

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

int USBTHING_get_firmware_version(struct usbthing_s *usbthing, char *version, int *length)
{
    int res;

    unsigned char version_str[USBTHING_CMD_FIRMWARE_GET_SIZE];
    int response_length;

    res = libusb_control_transfer (usbthing->handle,
                                   CONTROL_REQUEST_TYPE_IN,
                                   USBTHING_CMD_FIRMWARE_GET,
                                   0x00,
                                   0x00,
                                   version_str,
                                   USBTHING_CMD_FIRMWARE_GET_SIZE,
                                   USBTHING_TIMEOUT);

    if (res < 0) {
        perror("USBTHING get firmware version error");
    } else {
        printf("firmware: %s\n", version_str);
    }

    return res;
}

int USBTHING_led_set(struct usbthing_s *usbthing, int led, bool value)
{
    int res;

    res = libusb_control_transfer (usbthing->handle,
                                   CONTROL_REQUEST_TYPE_OUT,
                                   USBTHING_CMD_LED_SET,
                                   value,
                                   led,
                                   NULL,
                                   USBTHING_CMD_LED_SET_SIZE,
                                   USBTHING_TIMEOUT);
    //TODO: timeout

    if (res < 0) {
        perror("USBTHING led set error");
    }

    return res;
}

int USBTHING_gpio_configure(struct usbthing_s *usbthing, int pin, bool output, bool pull_enabled, bool pull_up)
{
    uint8_t mode;
    int res;

    //TODO: Sanity check mode input
    mode = 0;
    mode |= (output == true) ? USBTHING_GPIO_CFG_MODE_OUTPUT : USBTHING_GPIO_CFG_MODE_INPUT;
    mode |= (pull_enabled == true) ? USBTHING_GPIO_CFG_PULL_ENABLE : USBTHING_GPIO_CFG_PULL_DISABLE;
    mode |= (pull_up == true) ? USBTHING_GPIO_CFG_PULL_HIGH : USBTHING_GPIO_CFG_PULL_LOW;

    res = libusb_control_transfer (usbthing->handle,
                                   CONTROL_REQUEST_TYPE_OUT,
                                   USBTHING_CMD_GPIO_CFG,
                                   mode,
                                   pin,
                                   NULL,
                                   USBTHING_CMD_GPIO_CFG_SIZE,
                                   USBTHING_TIMEOUT);

    if (res < 0) {
        perror("USBTHING gpio configure error");
    }

    return res;
}


int USBTHING_gpio_set(struct usbthing_s *usbthing, int pin, bool value)
{
    int res;
    res = libusb_control_transfer (usbthing->handle,
                                   CONTROL_REQUEST_TYPE_OUT,
                                   USBTHING_CMD_GPIO_SET,
                                   value,
                                   pin,
                                   NULL,
                                   USBTHING_CMD_GPIO_SET_SIZE,
                                   USBTHING_TIMEOUT);

    if (res < 0) {
        perror("USBTHING gpio set error");
    }

    return res;
}

int USBTHING_gpio_get(struct usbthing_s *usbthing, int pin, bool *value)
{
    int res;
    unsigned char result[USBTHING_CMD_GPIO_GET_SIZE];

    res = libusb_control_transfer (usbthing->handle,
                                   LIBUSB_REQUEST_TYPE_VENDOR,
                                   USBTHING_CMD_GPIO_CFG,
                                   0,
                                   pin,
                                   result,
                                   USBTHING_CMD_GPIO_GET_SIZE,
                                   USBTHING_TIMEOUT);

    if (res < 0) {
        perror("USBTHING gpio get error");
    } else {
        (*value) = (result[0] == 0) ? false : true;
    }

    return res;
}

int USBTHING_spi_transfer(struct usbthing_s *usbthing, unsigned char *data_out, unsigned char *data_in, int length)
{
    int res;
    int transferred;

    libusb_clear_halt(usbthing->handle, 0x01);

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

    printf("Sent: %s\n", data_out);

    res = libusb_bulk_transfer (usbthing->handle,
                                0x81,
                                data_in,
                                length,
                                &transferred,
                                USBTHING_TIMEOUT);

    //TODO: check for complete transfer
    if (res < 0) {
        perror("USBTHING spi transfer incoming error");
        return -2;
    }

    printf("Received: %s\n", data_in);

    return 0;
}

int USBTHING_i2c_configure(struct usbthing_s *usbthing, int mode)
{
    int res;
    res = libusb_control_transfer (usbthing->handle,
                                   LIBUSB_REQUEST_TYPE_VENDOR,
                                   USBTHING_CMD_I2C_CFG,
                                   mode,
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

    printf("Sent: %s\n", data_out);

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

    printf("Received: %s\n", data_in);

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

    printf("Sent: %s\n", data_out);

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

    printf("Received: %s\n", data_in);

    return 0;
}

static void print_devs(libusb_device **devs, uint16_t vid_filter, uint16_t pid_filter)
{
    libusb_device *dev;
    int i = 0, j = 0;
    uint8_t path[8];

    printf("Devices:\n");

    while ((dev = devs[i++]) != NULL) {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            fprintf(stderr, "failed to get device descriptor");
            return;
        }

        if (((vid_filter == 0) || (vid_filter == desc.idVendor))
            && ((pid_filter == 0) || (pid_filter == desc.idVendor))) {

            printf("%04x:%04x (bus %d, device %d)",
                   desc.idVendor, desc.idProduct,
                   libusb_get_bus_number(dev), libusb_get_device_address(dev));

            r = libusb_get_port_numbers(dev, path, sizeof(path));
            if (r > 0) {
                printf(" path: %d", path[0]);
                for (j = 1; j < r; j++)
                    printf(".%d", path[j]);
            }
            printf("\n");

        }
    }
}