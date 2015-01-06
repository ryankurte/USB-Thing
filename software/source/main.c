
#include <stdio.h>
#include "usbthing.h"

int main(int argc, char **argv)
{
	USBTHING_init();

	USBTHING_list_devices();

	USBTHING_close();

	return 0;
}

