
#include <stdio.h>
#include "usbthing.h"

int main(int argc, char **argv)
{
	struct usbthing_s usbthing;
	int res;

	USBTHING_init();

	res = USBTHING_list_devices(0x0001, 0x0001);

setup:

	res = USBTHING_connect(&usbthing, 0x0001, 0x0001);
	if(res < 0) {
		printf("Error opening USB thing\n");
		goto teardown;
	}

	/* TODO: actual things */
	printf("Connected\n");

teardown:

	res = USBTHING_disconnect(&usbthing);
	if(res < 0) {
		printf("Error closing USB thing\n");
	}

	printf("Disconnected\n");

	USBTHING_close();

	return 0;
}

