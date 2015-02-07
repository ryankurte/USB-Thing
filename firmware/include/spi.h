
#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void SPI_init();

void SPI_transfer(uint8_t* data_out, uint8_t* data_in, int8_t length);

#endif
