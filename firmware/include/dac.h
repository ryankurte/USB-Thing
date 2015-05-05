#ifndef DAC_H
#define DAC_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void DAC_init();
void DAC_close();
void DAC_enable(bool enable);
void DAC_write(uint32_t value);

#ifdef __cplusplus
}
#endif

#endif