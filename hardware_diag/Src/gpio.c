#include <stdint.h>
#include "gpio.h"
#include "stm32l476_registers.h"

void GPIOA_PA5_Init(void)
{
    /* Enable GPIOA clock */
    RCC_AHB2ENR |= (1U << 0);

    /* Clear PA5 mode bits */
    GPIOA_MODER &= ~(3U << 10);

    /* Configure PA5 as general-purpose output */
    GPIOA_MODER |= (1U << 10);
}

void GPIOA_PA5_Set(void)
{
    GPIOA_BSRR = (1U << 5);
}

void GPIOA_PA5_Reset(void)
{
    GPIOA_BSRR = (1U << (5 + 16));
}

void GPIOA_PA5_Toggle(void)
{
    static uint8_t state = 0;

    state = !state;

    if (state)
    {
        GPIOA_PA5_Set();
    }
    else
    {
        GPIOA_PA5_Reset();
    }
}
