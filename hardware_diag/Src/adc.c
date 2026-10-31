#include "adc.h"
#include "stm32l476_registers.h"

static void ADC1_Delay(void)
{
    /*
     * Approximate delay for ADC voltage regulator startup.
     * Current project clock is 4 MHz.
     */
    for (volatile uint32_t i = 0; i < 1000U; i++)
    {
        __asm volatile ("nop");
    }
}

void ADC1_Init(void)
{
    /* -------------------------------------------------
       1. Enable GPIOA clock
       ------------------------------------------------- */
    RCC_AHB2ENR |= (1U << 0);

    /* -------------------------------------------------
       2. Configure PA0 as Analog mode
       MODER0[1:0] = 11
       ------------------------------------------------- */
    GPIOA_MODER &= ~(3U << 0);
    GPIOA_MODER |=  (3U << 0);

    /* -------------------------------------------------
       3. Enable ADC1 peripheral clock
       ADCEN = bit 13
       ------------------------------------------------- */
    RCC_AHB2ENR |= (1U << 13);

    /* -------------------------------------------------
       4. Configure ADC clock
       Use synchronous HCLK/1.

       ADC_CCR:
       CKMODE[1:0] = 01
       ------------------------------------------------- */
    ADC_CCR &= ~(3U << 16);
    ADC_CCR |=  (1U << 16);

    /* -------------------------------------------------
       5. Exit deep-power-down
       DEEPPWD = 0
       ------------------------------------------------- */
    ADC1_CR &= ~(1U << 29);

    /* -------------------------------------------------
       6. Enable ADC voltage regulator
       ADVREGEN = 1
       ------------------------------------------------- */
    ADC1_CR |= (1U << 28);

    /* Wait for voltage regulator startup */
    ADC1_Delay();

    /* -------------------------------------------------
       7. Select single-ended calibration
       ADCALDIF = 0
       ------------------------------------------------- */
    ADC1_CR &= ~(1U << 30);

    /* -------------------------------------------------
       8. Start ADC calibration
       ADCAL = 1
       ------------------------------------------------- */
    ADC1_CR |= (1U << 31);

    /* Wait until calibration finishes */
    while (ADC1_CR & (1U << 31))
    {
    }

    /* -------------------------------------------------
       9. Configure ADC

       CFGR = 0:
       12-bit resolution
       right aligned
       single conversion
       software trigger
       ------------------------------------------------- */
    ADC1_CFGR = 0;

    /* -------------------------------------------------
       10. Sampling time for channel 5

       SMP5 = 100
       = 47.5 ADC clock cycles

       SMP5 occupies bits 17:15
       ------------------------------------------------- */
    ADC1_SMPR1 &= ~(7U << 15);
    ADC1_SMPR1 |=  (4U << 15);

    /* -------------------------------------------------
       11. Select channel 5 as first conversion

       SQ1 occupies bits 10:6
       L = 0000 means one conversion
       ------------------------------------------------- */
    ADC1_SQR1 = 0;

    ADC1_SQR1 |= (5U << 6);

    /* -------------------------------------------------
       12. Clear ADRDY flag
       ------------------------------------------------- */
    ADC1_ISR = (1U << 0);

    /* -------------------------------------------------
       13. Enable ADC
       ADEN = 1
       ------------------------------------------------- */
    ADC1_CR |= (1U << 0);

    /* Wait until ADC is ready */
    while (!(ADC1_ISR & (1U << 0)))
    {
    }
}

uint16_t ADC1_Read(void)
{
    /* -------------------------------------------------
       Start regular conversion
       ADSTART = 1
       ------------------------------------------------- */
    ADC1_CR |= (1U << 2);

    /* -------------------------------------------------
       Wait for end of conversion
       EOC = bit 2
       ------------------------------------------------- */
    while (!(ADC1_ISR & (1U << 2)))
    {
    }

    /* Read converted 12-bit value */
    uint16_t value = (uint16_t)ADC1_DR;

    /* Clear EOC flag
       ADC ISR flags are cleared by writing 1 */
    ADC1_ISR = (1U << 2);

    return value;
}
