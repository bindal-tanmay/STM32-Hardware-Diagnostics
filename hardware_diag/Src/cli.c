#include "cli.h"

#include "uart.h"
#include "diagnostic.h"
#include "watchdog.h"

static void CLI_PrintPrompt(void)
{
    UART2_SendString("\r\n> ");
}

void CLI_Init(void)
{
    UART2_SendString("\r\n");
    UART2_SendString("STM32 Diagnostic CLI\r\n");
    UART2_SendString("Type 'help' for available commands.\r\n");

    CLI_PrintPrompt();
}

void CLI_Process(void)
{
    char command;

    if (UART2_ReceiveCharNonBlocking(&command))
    {
        /*
         * First version:
         * We will process single-character commands.
         * Full string command parsing will be added next.
         */

        switch (command)
        {
            case 'h':
                UART2_SendString("\r\nAvailable commands:\r\n");
                UART2_SendString("h - help\r\n");
                UART2_SendString("s - self test\r\n");
                UART2_SendString("p - SPI test\r\n");
                UART2_SendString("i - I2C test\r\n");
                UART2_SendString("a - ADC test\r\n");
                UART2_SendString("w - PWM test\r\n");
                UART2_SendString("d - DMA test\r\n");
                UART2_SendString("r - watchdog reset test\r\n");
                break;

            case 's':
                UART2_SendString("\r\nRunning self test...\r\n");
                Diagnostic_Run();
                break;

            case 'p':
                UART2_SendString("\r\n");
                Diagnostic_TestSPI();
                break;

            case 'i':
                UART2_SendString("\r\n");
                Diagnostic_TestI2C();
                break;

            case 'a':
                UART2_SendString("\r\n");
                Diagnostic_TestADC();
                break;

            case 'w':
                UART2_SendString("\r\n");
                Diagnostic_TestPWM();
                break;

            case 'd':
                UART2_SendString("\r\n");
                Diagnostic_TestDMA();
                break;

            case 'r':
                UART2_SendString("\r\n");
                UART2_SendString("[TEST] Watchdog reset test...\r\n");
                UART2_SendString("[INFO] MCU should reset in approximately 1 second\r\n");
                Watchdog_TestReset();
                break;

            case '\r':
            case '\n':
                break;

            default:
                UART2_SendString("\r\nUnknown command\r\n");
                break;
        }

        CLI_PrintPrompt();
    }
}
