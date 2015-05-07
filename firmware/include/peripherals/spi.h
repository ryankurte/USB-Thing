
#ifndef SPI_H
#define SPI_H

#include <stdint.h>

int8_t SPI_init(uint32_t baud, uint8_t clock_mode);
int8_t SPI_transfer(uint8_t length, uint8_t *data_out, uint8_t *data_in);

#endif
