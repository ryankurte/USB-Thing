
#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C_init(uint32_t baud);
int8_t I2C_write(uint8_t address, uint32_t num_bytes, const uint8_t *data_array);
int8_t I2C_read(uint8_t address, uint32_t num_bytes, uint8_t *data_array);
int8_t I2C_write_read(uint8_t address, uint32_t num_write_bytes, const uint8_t *write_data_array, uint32_t num_read_bytes, uint8_t *read_data_array);

#endif
