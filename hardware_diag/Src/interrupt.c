#include "interrupt.h"
#include "stm32l476_registers.h"

static volatile uint8_t button_event = 0;

void EXTI13_Init(void)
{
    /* Enable GPIOC clock */
    RCC_AHB2ENR |= (1U << 2);

    /* PC13 as input */
    GPIOC_MODER &= ~(3U << 26);
    /* Enable internal pull-up on PC13 */
    GPIOC_PUPDR &= ~(3U << 26);
    GPIOC_PUPDR |=  (1U << 26);

    /* Enable SYSCFG clock */
    RCC_APB2ENR |= (1U << 0);

    SYSCFG_EXTICR4 &= ~(0xFU << 4);
    SYSCFG_EXTICR4 |=  (2U << 4);

    /* Unmask EXTI13 */
    EXTI_IMR1 |= (1U << 13);

    /* Falling-edge trigger */
    EXTI_RTSR1 &= ~(1U << 13);
    EXTI_FTSR1 |=  (1U << 13);

    /* Clear any pending interrupt */
    EXTI_PR1 = (1U << 13);

    /* Enable EXTI15_10 interrupt in NVIC */
    NVIC_ISER1 |= (1U << 8);
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI_PR1 & (1U << 13))
    {
        /* Clear interrupt pending flag */
        EXTI_PR1 = (1U << 13);

        /* Notify main application */
        button_event = 1;
    }
}

uint8_t Interrupt_ButtonEvent(void)
{
    if (button_event)
    {
        button_event = 0;
        return 1;
    }

    return 0;
}
