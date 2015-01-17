
#include <stdio.h>
#include "usbthing.h"

int main(int argc, char **argv)
{
	struct usbthing_s usbthing;
	int res;

	USBTHING_init();

	USBTHING_list_devices(0x0001, 0x0001);

	res = USBTHING_connect(&usbthing, 0x0001, 0x0001);
	if(res < 0) {
		printf("Error opening USB thing\n");
	}

	/* TODO: actual things */
	printf("Connected\n");

	res = USBTHING_disconnect(&usbthing);
	if(res < 0) {
		printf("Error closing USB thing\n");
	}

	printf("Disconnected\n");

	USBTHING_close();

	return 0;
}

