#include "uart.h"
#include "stm32l476_registers.h"

#define UART_BAUDRATE 9600U
#define UART_CLOCK    4000000U

void UART2_Init(void)
{
    /* Enable GPIOA clock */
    RCC_AHB2ENR |= (1U << 0);

    /* Enable USART2 clock */
    RCC_APB1ENR1 |= (1U << 17);

    /* PA2 and PA3 -> Alternate Function mode */
    GPIOA_MODER &= ~((3U << 4) | (3U << 6));
    GPIOA_MODER |=  ((2U << 4) | (2U << 6));

    /* PA2 and PA3 -> AF7 (USART2) */
    GPIOA_AFRL &= ~((0xFU << 8) | (0xFU << 12));
    GPIOA_AFRL |=  ((7U << 8) | (7U << 12));

    /* Configure baud rate */
    USART2_BRR = (UART_CLOCK + (UART_BAUDRATE / 2U))
                 / UART_BAUDRATE;

    /* Enable transmitter and receiver */
    USART2_CR1 = (1U << 3) | (1U << 2);

    /* Enable USART2 */
    USART2_CR1 |= (1U << 0);
}


void UART2_SendChar(char c)
{
    /* Wait until transmit data register is empty */
    while (!(USART2_ISR & (1U << 7)))
    {
    }

    /* Put character into transmit data register */
    USART2_TDR = (uint32_t)c;
}


void UART2_SendString(const char *str)
{
    while (*str)
    {
        UART2_SendChar(*str);
        str++;
    }
}



char UART2_ReceiveChar(void)
{
    /* Wait until data is received */
    while (!(USART2_ISR & (1U << 5)))
    {
    }

    /* Read received character */
    return (char)USART2_RDR;
}

int UART2_ReceiveCharNonBlocking(char *c)
{
    if (USART2_ISR & (1U << 5))
    {
        *c = (char)USART2_RDR;
        return 1;
    }

    return 0;
}
