#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void ADC_init();
void ADC_close();
void ADC_enable(uint8_t channel, bool enable);
uint32_t ADC_read(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif