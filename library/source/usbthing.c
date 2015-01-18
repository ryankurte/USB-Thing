/**
 * @brief USB Thing Interface Library
 * @details [long description]
 *
 * see: http://libusb.sourceforge.net/api-1.0
 */

#include "usbthing.h"

#include <stdio.h>
#include <stdint.h>
#include "libusb-1.0/libusb.h"

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

    usbthing->handle = libusb_open_device_with_vid_pid(NULL, vid_filter, pid_filter);

    if (usbthing->handle == NULL) {
        //Device not found (or error)
        return -1;
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

    unsigned char version_str[USBTHING_FIRMWARE_MAX_SIZE];
    int response_length;

    res = libusb_control_transfer (usbthing->handle,
                             LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT,
                             USBTHING_CMD_LED_SET,
                             0x00,
                             0x00,
                             version_str,
                             4,
                             1000);

    if(res < 0) {
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
                             LIBUSB_REQUEST_TYPE_VENDOR,
                             USBTHING_CMD_LED_SET,
                             led,
                             value,
                             NULL,
                             0,
                             1000);
    //TODO: timeout

    if(res < 0) {
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
                             LIBUSB_REQUEST_TYPE_VENDOR,
                             USBTHING_CMD_GPIO_CFG,
                             pin,
                             mode,
                             NULL,
                             0,
                             1000);
    //TODO: timeout

    if(res < 0) {
        perror("USBTHING gpio configure error");
    }

    return res;
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