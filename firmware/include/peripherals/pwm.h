
#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void PWM_init();
void PWM_close();
void PWM_enable(uint8_t channel, bool enable);
void PWM_write(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif
