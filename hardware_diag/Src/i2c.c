#include "i2c.h"
#include "stm32l476_registers.h"

void I2C1_Init(void)
{
    /* Enable GPIOB clock */
    RCC_AHB2ENR |= (1U << 1);

    /* PB8 and PB9 -> Alternate Function mode */
    GPIOB_MODER &= ~((3U << 16) | (3U << 18));
    GPIOB_MODER |=  ((2U << 16) | (2U << 18));

    /* PB8 and PB9 -> Open drain */
    GPIOB_OTYPER |= (1U << 8) | (1U << 9);

    /* PB8 and PB9 -> Pull-up */
    GPIOB_PUPDR &= ~((3U << 16) | (3U << 18));
    GPIOB_PUPDR |=  ((1U << 16) | (1U << 18));

    /* PB8 and PB9 -> AF4 (I2C1) */
    GPIOB_AFRH &= ~((0xFU << 0) | (0xFU << 4));
    GPIOB_AFRH |=  ((4U << 0) | (4U << 4));

    /* Enable I2C1 clock */
    RCC_APB1ENR1 |= (1U << 21);

    /* Disable I2C before configuring */
    I2C1_CR1 &= ~(1U << 0);

    /*
     * I2C clock = 4 MHz
     * I2C speed ≈ 100 kHz
     */
    I2C1_TIMINGR = 0x00420F13UL;

    /* Enable I2C1 */
    I2C1_CR1 |= (1U << 0);
}

uint8_t I2C1_DevicePresent(uint8_t address)
{
    /* Clear previous status flags */
    I2C1_ICR = (1U << 4) | (1U << 5);

    /*
     * 7-bit address goes into CR2[7:1]
     */
    I2C1_CR2 = ((uint32_t)address << 1);

    /*
     * NBYTES = 0
     * AUTOEND = 1
     * START = 1
     */
    I2C1_CR2 |= (1U << 25);
    I2C1_CR2 |= (1U << 13);

    /* Wait for NACK or STOP */
    while (1)
    {
        uint32_t status = I2C1_ISR;

        /* Device did not acknowledge */
        if (status & (1U << 4))
        {
            I2C1_ICR = (1U << 4);
            return 0;
        }

        /* Device acknowledged */
        if (status & (1U << 5))
        {
            I2C1_ICR = (1U << 5);
            return 1;
        }
    }
}

uint8_t I2C1_Write(uint8_t address, const uint8_t *data, uint8_t length)
{
    uint32_t status;

    /* Clear previous flags */
    I2C1_ICR = (1U << 4) |    /* NACKCF */
               (1U << 5) |    /* STOPCF */
               (1U << 3);     /* BERRCF */

    /*
     * 7-bit slave address -> CR2[7:1]
     */
    I2C1_CR2 = ((uint32_t)address << 1);

    /*
     * Number of bytes
     */
    I2C1_CR2 |= ((uint32_t)length << 16);

    /*
     * AUTOEND = 1
     */
    I2C1_CR2 |= (1U << 25);

    /*
     * START = 1
     */
    I2C1_CR2 |= (1U << 13);

    for (uint8_t i = 0; i < length; i++)
    {
        /* Wait for TXIS, STOPF or NACKF */
        while (1)
        {
            status = I2C1_ISR;

            if (status & (1U << 4))
            {
                /* NACK */
                I2C1_ICR = (1U << 4);
                return 0;
            }

            if (status & (1U << 5))
            {
                /* Unexpected STOP */
                I2C1_ICR = (1U << 5);
                return 0;
            }

            if (status & (1U << 1))
            {
                /* TXIS */
                break;
            }
        }

        I2C1_TXDR = data[i];
    }

    /* Wait for STOP */
    while (!(I2C1_ISR & (1U << 5)))
    {
        if (I2C1_ISR & (1U << 4))
        {
            I2C1_ICR = (1U << 4);
            return 0;
        }
    }

    /* Clear STOP flag */
    I2C1_ICR = (1U << 5);

    return 1;
}
