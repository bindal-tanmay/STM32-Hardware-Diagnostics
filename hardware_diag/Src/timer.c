#include "timer.h"
#include "stm32l476_registers.h"

void TIM2_Init(void)
{
    /* Enable TIM2 clock */
    RCC_APB1ENR1 |= (1U << 0);

    /*
     * Timer clock = 4 MHz
     *
     * Prescaler = 3999
     *
     * 4 MHz / 4000 = 1 kHz
     *
     * Therefore:
     * 1 timer tick = 1 ms
     */
    TIM2_PSC = 3999U;

    /*
     * 1000 timer ticks = 1 second
     */
    TIM2_ARR = 999U;

    /* Generate an update event to load the registers */
    TIM2_EGR = (1U << 0);

    /* Clear update flag */
    TIM2_SR &= ~(1U << 0);

    /* Start timer */
    TIM2_CR1 |= (1U << 0);
}

uint8_t TIM2_UpdateOccurred(void)
{
    if (TIM2_SR & (1U << 0))
    {
        TIM2_SR &= ~(1U << 0);
        return 1;
    }

    return 0;
}

void TIM2_PWM_Init(void)
{
    /* Enable GPIOA clock */
    RCC_AHB2ENR |= (1U << 0);

    /* Configure PA1 as Alternate Function */
    GPIOA_MODER &= ~(3U << 2);
    GPIOA_MODER |=  (2U << 2);

    /* PA1 → AF1 → TIM2_CH2 */
    GPIOA_AFRL &= ~(0xFU << 4);
    GPIOA_AFRL |=  (1U << 4);

    /* Enable TIM2 clock */
    RCC_APB1ENR1 |= (1U << 0);

    /* Stop timer while configuring */
    TIM2_CR1 &= ~(1U << 0);

    /*
     * Timer clock = 4 MHz
     *
     * 4 MHz / (3 + 1) = 1 MHz
     */
    TIM2_PSC = 3U;

    /*
     * 1 MHz / (999 + 1) = 1 kHz PWM
     */
    TIM2_ARR = 999U;

    /*
     * Configure Channel 2:
     *
     * CC2S  = 00 → output
     * OC2PE = 1  → preload enabled
     * OC2M  = 1100 → PWM mode 1
     */

    TIM2_CCMR1 = 0;

    /* OC2PE = bit 11 */
    TIM2_CCMR1 |= (1U << 11);

    /*
     * OC2M[2:0] = 100
     * OC2M[3]   = 1
     *
     * Together = 1100 = PWM mode 1
     */
    TIM2_CCMR1 |= (4U << 12);
    TIM2_CCMR1 |= (1U << 24);

    /* Initial duty cycle = 50% */
    TIM2_CCR2 = 500U;

    /*
     * CC2E = bit 4
     * Enable TIM2_CH2 output
     */
    TIM2_CCER |= (1U << 4);

    /*
     * Generate update event so PSC/ARR
     * values are loaded.
     */
    TIM2_EGR = (1U << 0);

    /* Start timer */
    TIM2_CR1 |= (1U << 0);
}

void TIM2_PWM_SetDuty(uint8_t duty)
{
    if (duty > 100U)
        duty = 100U;

    TIM2_CCR2 = ((uint32_t)(TIM2_ARR + 1U) * duty) / 100U;
}
