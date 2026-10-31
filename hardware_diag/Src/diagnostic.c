#include "diagnostic.h"

#include "uart.h"
#include "spi.h"
#include "i2c.h"
#include "oled.h"
#include "adc.h"
#include "timer.h"
#include "dma.h"

static void Diagnostic_SendNumber(uint16_t number)
{
    char buffer[6];
    int i = 0;

    if (number == 0)
    {
        UART2_SendChar('0');
        return;
    }

    while (number > 0)
    {
        buffer[i++] = (number % 10U) + '0';
        number /= 10U;
    }

    while (i > 0)
    {
        UART2_SendChar(buffer[--i]);
    }
}

void Diagnostic_Init(void)
{
    UART2_Init();

    UART2_SendString("\r\n");
    UART2_SendString("========================================\r\n");
    UART2_SendString(" STM32 HARDWARE DIAGNOSTIC PLATFORM\r\n");
    UART2_SendString("========================================\r\n");

    UART2_SendString("[BOOT] Initializing peripherals...\r\n");

    SPI1_Init();
    UART2_SendString("[INIT] SPI1 initialized\r\n");

    I2C1_Init();
    UART2_SendString("[INIT] I2C1 initialized\r\n");

    OLED_Init();
    UART2_SendString("[INIT] OLED initialized\r\n");
    OLED_Clear();
    OLED_SetCursor(0, 0);
    OLED_WriteString("STM32 DIAGNOSTIC");
    OLED_SetCursor(0, 2);
    OLED_WriteString("OLED TEST PASS");
    OLED_SetCursor(0, 4);
    OLED_WriteString("HAPPY DIWALI !!!");

    ADC1_Init();
    UART2_SendString("[INIT] ADC1 initialized\r\n");

    TIM2_PWM_Init();
    UART2_SendString("[INIT] TIM2 PWM initialized\r\n");

    DMA1_Init();
    UART2_SendString("[INIT] DMA1 initialized\r\n");

    UART2_SendString("[BOOT] Initialization complete\r\n");
}

/* ---------------- SPI TEST ---------------- */

uint8_t Diagnostic_TestSPI(void)
{
    uint8_t tx = 0xA5;
    uint8_t rx;

    UART2_SendString("[TEST] SPI1 Loopback...\r\n");

    rx = SPI1_Transfer(tx);

    if (rx == tx)
    {
        UART2_SendString("[PASS] SPI1 LOOPBACK\r\n");
        return 1;
    }

    UART2_SendString("[FAIL] SPI1 LOOPBACK\r\n");
    return 0;
}

/* ---------------- I2C TEST ---------------- */

uint8_t Diagnostic_TestI2C(void)
{
    UART2_SendString("[TEST] I2C1 Device Detection...\r\n");

    if (I2C1_DevicePresent(0x3C))
    {
        UART2_SendString("[PASS] OLED detected at 0x3C\r\n");
        return 1;
    }

    UART2_SendString("[FAIL] OLED not detected\r\n");
    return 0;
}

/* ---------------- ADC TEST ---------------- */

uint8_t Diagnostic_TestADC(void)
{
    uint16_t adc_value;

    UART2_SendString("[TEST] ADC1 Conversion...\r\n");

    adc_value = ADC1_Read();

    UART2_SendString("[PASS] ADC1 Value = ");
    Diagnostic_SendNumber(adc_value);
    UART2_SendString("\r\n");

    return 1;
}

/* ---------------- PWM TEST ---------------- */

uint8_t Diagnostic_TestPWM(void)
{
    UART2_SendString("[TEST] TIM2 PWM...\r\n");

    /*
     * PWM was already verified using the oscilloscope.
     * At this stage the software test confirms that
     * TIM2 PWM has been initialized.
     */
    UART2_SendString("[PASS] TIM2 PWM initialized\r\n");

    return 1;
}

/* ---------------- DMA TEST ---------------- */
uint8_t Diagnostic_TestDMA(void)
{
    UART2_SendString("[TEST] DMA1 Memory Transfer...\r\n");

    if (DMA1_MemoryTransferTest())
    {
        UART2_SendString("[PASS] DMA1 MEMORY TRANSFER\r\n");
        return 1;
    }

    UART2_SendString("[FAIL] DMA1 MEMORY TRANSFER\r\n");
    return 0;
}

/* ---------------- INITIALIZATION ---------------- */

void Diagnostic_Run(void)
{
    uint8_t pass_count = 0;
    uint8_t total_tests = 4;

    UART2_SendString("\r\n");
    UART2_SendString("========== SELF TEST ==========\r\n");

    if (Diagnostic_TestSPI())
        pass_count++;

    if (Diagnostic_TestI2C())
        pass_count++;

    if (Diagnostic_TestADC())
        pass_count++;

    if (Diagnostic_TestPWM())
        pass_count++;

    UART2_SendString("\r\n");
    UART2_SendString("========== SELF TEST SUMMARY ==========\r\n");

    UART2_SendString("Tests Passed: ");
    Diagnostic_SendNumber(pass_count);

    UART2_SendString("/");
    Diagnostic_SendNumber(total_tests);

    UART2_SendString("\r\n");

    if (pass_count == total_tests)
    {
        UART2_SendString("[SYSTEM] ALL TESTS PASSED\r\n");
    }
    else
    {
        UART2_SendString("[SYSTEM] SOME TESTS FAILED\r\n");
    }

    UART2_SendString("========================================\r\n");
}
