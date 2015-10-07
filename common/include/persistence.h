

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#define MAX_SERIAL_SIZE		32
#define MAX_VERSION_SIZE	32

#include <stdint.h>

/**
 * Structure for persistent storage in the device flash
 * Used to store information about the device and the loaded application
 * version (though this is also baked into the application)
 */
struct persistence_s {
	uint8_t serial[MAX_SERIAL_SIZE];
	uint8_t app_version[MAX_VERSION_SIZE];
	uint32_t app_crc;
};

#endif
