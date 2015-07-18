#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void ADC_init(uint32_t reference);
void ADC_close();
void ADC_enable(uint8_t channel, bool enable);
uint32_t ADC_get(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif