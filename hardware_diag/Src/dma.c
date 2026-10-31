#include "dma.h"
#include "stm32l476_registers.h"

static volatile uint8_t dma_source[8] =
{
    1, 2, 3, 4, 5, 6, 7, 8
};

static volatile uint8_t dma_destination[8] =
{
    0, 0, 0, 0, 0, 0, 0, 0
};

void DMA1_Init(void)
{
    /* Enable DMA1 clock */
    RCC_AHB1ENR |= (1U << 0);

    /* Disable DMA channel before configuration */
    DMA1_CCR1 &= ~(1U << 0);

    /* Clear DMA1 Channel 1 flags */
    DMA1_IFCR = 0x0FU;

    /*
     * Memory-to-memory transfer
     *
     * MEM2MEM = bit 14
     * MINC    = bit 7
     * PINC    = bit 6
     *
     * PSIZE = 00 -> 8-bit
     * MSIZE = 00 -> 8-bit
     *
     * DIR = 0
     * CPAR = source
     * CMAR = destination
     */
    DMA1_CCR1 = 0;

    DMA1_CCR1 |= (1U << 14);  /* MEM2MEM */
    DMA1_CCR1 |= (1U << 7);   /* MINC */
    DMA1_CCR1 |= (1U << 6);   /* PINC */

    /* Number of bytes */
    DMA1_CNDTR1 = 8U;

    /* Source address */
    DMA1_CPAR1 = (uint32_t)dma_source;

    /* Destination address */
    DMA1_CMAR1 = (uint32_t)dma_destination;

    /* Enable DMA channel */
    DMA1_CCR1 |= (1U << 0);
}

uint8_t DMA1_MemoryTransferTest(void)
{
    uint8_t i;

    /* Reset destination */
    for (i = 0; i < 8U; i++)
    {
        dma_destination[i] = 0;
    }

    /* Disable channel */
    DMA1_CCR1 &= ~(1U << 0);

    /* Clear flags */
    DMA1_IFCR = 0x0FU;

    /* Configure transfer */
    DMA1_CNDTR1 = 8U;
    DMA1_CPAR1 = (uint32_t)dma_source;
    DMA1_CMAR1 = (uint32_t)dma_destination;

    /* Start DMA */
    DMA1_CCR1 |= (1U << 0);

    /* Wait for Transfer Complete */
    while (!(DMA1_ISR & (1U << 1)))
    {
    }

    /* Disable DMA channel */
    DMA1_CCR1 &= ~(1U << 0);

    /* Verify destination */
    for (i = 0; i < 8U; i++)
    {
        if (dma_destination[i] != dma_source[i])
        {
            return 0;
        }
    }

    /* Clear flags */
    DMA1_IFCR = 0x0FU;

    return 1;
}
