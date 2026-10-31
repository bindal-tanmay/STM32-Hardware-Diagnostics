#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void TIM2_Init(void);
uint8_t TIM2_UpdateOccurred(void);

void TIM2_PWM_Init(void);
void TIM2_PWM_SetDuty(uint8_t duty);

#endif
