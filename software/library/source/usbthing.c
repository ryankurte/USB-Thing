

#include "usbthing.h"

#include "libusb.h"

static void print_devs(libusb_device **devs)

void USBTHING_init()
{
    int res;
    ssize_t cnt;

    res = libusb_init(NULL);
    if (res < 0)
    {
        return res;
    }

    return 0;
}

void USBTHING_close()
{
    libusb_exit(NULL);
}

void USBTHING_list_devices()
{
    libusb_device **devs;

    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0)
        return (int) cnt;

    print_devs(devs);
    libusb_free_device_list(devs, 1);
}

static void print_devs(libusb_device **devs)
{
    libusb_device *dev;
    int i = 0, j = 0;
    uint8_t path[8];

    while ((dev = devs[i++]) != NULL)
    {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0)
        {
            fprintf(stderr, "failed to get device descriptor");
            return;
        }

        printf("%04x:%04x (bus %d, device %d)",
               desc.idVendor, desc.idProduct,
               libusb_get_bus_number(dev), libusb_get_device_address(dev));

        r = libusb_get_port_numbers(dev, path, sizeof(path));
        if (r > 0)
        {
            printf(" path: %d", path[0]);
            for (j = 1; j < r; j++)
                printf(".%d", path[j]);
        }
        printf("\n");
    }
}