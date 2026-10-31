#ifndef STM32L476_REGISTERS_H
#define STM32L476_REGISTERS_H

#include <stdint.h>

/* RCC */
#define RCC_BASE        0x40021000UL
#define RCC_AHB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x4CUL))

/* GPIOA */
#define GPIOA_BASE      0x48000000UL
#define GPIOA_MODER     (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_BSRR      (*(volatile uint32_t *)(GPIOA_BASE + 0x18UL))


#define GPIOA_AFRL      (*(volatile uint32_t *)(GPIOA_BASE + 0x20UL))



/* USART2 */
#define USART2_BASE     0x40004400UL

#define USART2_CR1      (*(volatile uint32_t *)(USART2_BASE + 0x00UL))
#define USART2_BRR      (*(volatile uint32_t *)(USART2_BASE + 0x0CUL))
#define USART2_ISR      (*(volatile uint32_t *)(USART2_BASE + 0x1CUL))
#define USART2_RDR      (*(volatile uint32_t *)(USART2_BASE + 0x24UL))
#define USART2_TDR      (*(volatile uint32_t *)(USART2_BASE + 0x28UL))

/* RCC APB1 peripheral clock enable */
#define RCC_APB1ENR1    (*(volatile uint32_t *)0x40021058UL)

/* TIM2 */
#define TIM2_BASE       0x40000000UL

#define TIM2_CR1        (*(volatile uint32_t *)(TIM2_BASE + 0x00UL))
#define TIM2_SR         (*(volatile uint32_t *)(TIM2_BASE + 0x10UL))
#define TIM2_EGR        (*(volatile uint32_t *)(TIM2_BASE + 0x14UL))
#define TIM2_CNT        (*(volatile uint32_t *)(TIM2_BASE + 0x24UL))
#define TIM2_PSC        (*(volatile uint32_t *)(TIM2_BASE + 0x28UL))
#define TIM2_ARR        (*(volatile uint32_t *)(TIM2_BASE + 0x2CUL))


/* RCC APB2 peripheral clock enable */
#define RCC_APB2ENR    (*(volatile uint32_t *)0x40021060UL)

/* SPI1 */
#define SPI1_BASE      0x40013000UL
#define SPI1_CR1       (*(volatile uint32_t *)(SPI1_BASE + 0x00UL))
#define SPI1_CR2       (*(volatile uint32_t *)(SPI1_BASE + 0x04UL))
#define SPI1_SR        (*(volatile uint32_t *)(SPI1_BASE + 0x08UL))
#define SPI1_DR        (*(volatile uint32_t *)(SPI1_BASE + 0x0CUL))


/* GPIOB */
#define GPIOB_BASE        0x48000400UL
#define GPIOB_MODER       (*(volatile uint32_t *)(GPIOB_BASE + 0x00UL))
#define GPIOB_OTYPER      (*(volatile uint32_t *)(GPIOB_BASE + 0x04UL))
#define GPIOB_PUPDR       (*(volatile uint32_t *)(GPIOB_BASE + 0x0CUL))
#define GPIOB_AFRH        (*(volatile uint32_t *)(GPIOB_BASE + 0x24UL))

/* I2C1 */
#define I2C1_BASE         0x40005400UL
#define I2C1_CR1          (*(volatile uint32_t *)(I2C1_BASE + 0x00UL))
#define I2C1_CR2          (*(volatile uint32_t *)(I2C1_BASE + 0x04UL))
#define I2C1_TIMINGR      (*(volatile uint32_t *)(I2C1_BASE + 0x10UL))
#define I2C1_ISR          (*(volatile uint32_t *)(I2C1_BASE + 0x18UL))
#define I2C1_ICR          (*(volatile uint32_t *)(I2C1_BASE + 0x1CUL))
#define I2C1_TXDR         (*(volatile uint32_t *)(I2C1_BASE + 0x28UL))
#define I2C1_RXDR         (*(volatile uint32_t *)(I2C1_BASE + 0x24UL))

/* ADC1 */
#define ADC1_BASE       0x50040000UL

#define ADC1_ISR        (*(volatile uint32_t *)(ADC1_BASE + 0x00UL))
#define ADC1_CR         (*(volatile uint32_t *)(ADC1_BASE + 0x08UL))
#define ADC1_CFGR       (*(volatile uint32_t *)(ADC1_BASE + 0x0CUL))
#define ADC1_SMPR1      (*(volatile uint32_t *)(ADC1_BASE + 0x14UL))
#define ADC1_SQR1       (*(volatile uint32_t *)(ADC1_BASE + 0x30UL))
#define ADC1_DR         (*(volatile uint32_t *)(ADC1_BASE + 0x40UL))

/* ADC common registers */
#define ADC_COMMON_BASE 0x50040300UL
#define ADC_CCR         (*(volatile uint32_t *)(ADC_COMMON_BASE + 0x08UL))

#define RCC_CCIPR       (*(volatile uint32_t *)(RCC_BASE + 0x88UL))


#define TIM2_CCMR1     (*(volatile uint32_t *)(TIM2_BASE + 0x18UL))
#define TIM2_CCER      (*(volatile uint32_t *)(TIM2_BASE + 0x20UL))
#define TIM2_CCR2      (*(volatile uint32_t *)(TIM2_BASE + 0x38UL))

/* GPIOC */

#define GPIOC_BASE        0x48000800UL
#define GPIOC_MODER       (*(volatile uint32_t *)(GPIOC_BASE + 0x00UL))
#define GPIOC_PUPDR       (*(volatile uint32_t *)(GPIOC_BASE + 0x0CUL))


/* SYSCFG */
#define SYSCFG_BASE       0x40010000UL
#define SYSCFG_EXTICR4    (*(volatile uint32_t *)(SYSCFG_BASE + 0x14UL))

/* EXTI */
#define EXTI_BASE         0x40010400UL
#define EXTI_IMR1         (*(volatile uint32_t *)(EXTI_BASE + 0x00UL))
#define EXTI_RTSR1        (*(volatile uint32_t *)(EXTI_BASE + 0x08UL))
#define EXTI_FTSR1        (*(volatile uint32_t *)(EXTI_BASE + 0x0CUL))
#define EXTI_PR1          (*(volatile uint32_t *)(EXTI_BASE + 0x14UL))

/* NVIC */
#define NVIC_ISER1        (*(volatile uint32_t *)0xE000E104UL)

/* DMA1 */
#define DMA1_BASE       0x40020000UL

#define DMA1_ISR        (*(volatile uint32_t *)(DMA1_BASE + 0x00UL))
#define DMA1_IFCR       (*(volatile uint32_t *)(DMA1_BASE + 0x04UL))

/* DMA1 Channel 1 */
#define DMA1_CCR1       (*(volatile uint32_t *)(DMA1_BASE + 0x08UL))
#define DMA1_CNDTR1     (*(volatile uint32_t *)(DMA1_BASE + 0x0CUL))
#define DMA1_CPAR1      (*(volatile uint32_t *)(DMA1_BASE + 0x10UL))
#define DMA1_CMAR1      (*(volatile uint32_t *)(DMA1_BASE + 0x14UL))

/* DMA channel selection */
#define DMA1_CSELR      (*(volatile uint32_t *)(DMA1_BASE + 0xA8UL))

#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x48UL))

/* Independent Watchdog (IWDG) */
#define IWDG_BASE       0x40003000UL

#define IWDG_KR         (*(volatile uint32_t *)(IWDG_BASE + 0x00UL))
#define IWDG_PR         (*(volatile uint32_t *)(IWDG_BASE + 0x04UL))
#define IWDG_RLR        (*(volatile uint32_t *)(IWDG_BASE + 0x08UL))
#define IWDG_SR         (*(volatile uint32_t *)(IWDG_BASE + 0x0CUL))

#endif
