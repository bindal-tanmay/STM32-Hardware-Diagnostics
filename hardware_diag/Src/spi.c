#include "spi.h"
#include "stm32l476_registers.h"

void SPI1_Init(void)
{
    /* Enable GPIOA clock */
    RCC_AHB2ENR |= (1U << 0);

    /* PA5, PA6, PA7 -> Alternate Function mode (10) */
    GPIOA_MODER &= ~((3U << 10) |
                     (3U << 12) |
                     (3U << 14));

    GPIOA_MODER |= ((2U << 10) |
                    (2U << 12) |
                    (2U << 14));

    /* PA5, PA6, PA7 -> AF5 (SPI1) */
    GPIOA_AFRL &= ~((0xFU << 20) |
                    (0xFU << 24) |
                    (0xFU << 28));

    GPIOA_AFRL |= ((5U << 20) |
                   (5U << 24) |
                   (5U << 28));

    /* Enable SPI1 clock on APB2 */
    RCC_APB2ENR |= (1U << 12);

    /* Configure SPI1 */
    SPI1_CR1 = 0;

    /* Master mode */
    SPI1_CR1 |= (1U << 2);

    /* Baud rate: PCLK2 / 8 = 4 MHz / 8 = 500 kHz
       BR[2:0] = 010 */
    SPI1_CR1 |= (2U << 3);

    /* Software slave management */
    SPI1_CR1 |= (1U << 9);

    /* Internal NSS high */
    SPI1_CR1 |= (1U << 8);

    /* Enable SPI */
    SPI1_CR1 |= (1U << 6);
}

uint8_t SPI1_Transfer(uint8_t data)
{
    /* Wait until transmit buffer is empty */
    while (!(SPI1_SR & (1U << 1)))
    {
    }

    /* Write data */
    SPI1_DR = data;

    /* Wait until received data is available */
    while (!(SPI1_SR & (1U << 0)))
    {
    }

    /* Return received data */
    return (uint8_t)SPI1_DR;
}
