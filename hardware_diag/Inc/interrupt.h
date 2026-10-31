#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

void EXTI13_Init(void);
uint8_t Interrupt_ButtonEvent(void);

#endif
