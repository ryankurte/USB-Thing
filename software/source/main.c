
#include <stdio.h>
#include <unistd.h>

#include "usbthing.h"
#include "selftest.h"

int main(int argc, char **argv)
{
    struct usbthing_s usbthing;
    char version[32];
    int res;

    USBTHING_init();

    res = USBTHING_list_devices(0x0001, 0x0001);

setup:

    res = USBTHING_connect(&usbthing, 0x0001, 0x0001);
    if (res < 0) {
        printf("Error opening USB thing\n");
        goto teardown;
    }

    /* TODO: actual things */
    printf("Connected\n");

    USBTHING_get_firmware_version(&usbthing, sizeof(version), version);

    printf("Firmware version: %s\r\n", version);

    //USBTHING_i2c_configure(&usbthing, USBTHING_I2C_SPEED_STANDARD);

    //USBTHING_i2c_write_read(&usbthing, 0x00, sizeof(data_out), data_out, sizeof(data_in), data_in);

    printf("Running self tests\r\n");

    self_test(&usbthing, 0);

teardown:

    res = USBTHING_disconnect(&usbthing);
    if (res < 0) {
        printf("Error closing USB thing\n");
    }

    printf("Disconnected\n");

    USBTHING_close();

    return 0;
}

