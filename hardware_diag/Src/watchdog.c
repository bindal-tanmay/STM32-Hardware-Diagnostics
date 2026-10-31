#include "watchdog.h"
#include "stm32l476_registers.h"

void Watchdog_Init(void)
{
    /*
     * Unlock IWDG prescaler and reload registers
     */
    IWDG_KR = 0x5555U;

    /*
     * IWDG clock = LSI
     *
     * Prescaler = 256
     */
    IWDG_PR = 6U;

    /*
     * Reload value
     *
     * Approximately 1 second timeout
     * assuming LSI is around 32 kHz.
     */
    IWDG_RLR = 125U;

    /*
     * Wait until prescaler/reload update is complete
     */
    while (IWDG_SR != 0U)
    {
    }
}

void Watchdog_Refresh(void)
{
    /*
     * Reload the watchdog counter
     */
    IWDG_KR = 0xAAAAU;
}

uint8_t Watchdog_TestReset(void)
{
    /*
     * Start the Independent Watchdog
     */
    IWDG_KR = 0xCCCCU;

    /*
     * Intentionally do NOT refresh the watchdog.
     *
     * The MCU should reset automatically
     * when the watchdog timeout expires.
     */
    while (1)
    {
    }
}
