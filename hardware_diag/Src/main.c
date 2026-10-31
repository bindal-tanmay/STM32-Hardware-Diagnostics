#include "diagnostic.h"
#include "cli.h"
#include "interrupt.h"
#include "uart.h"

int main(void)
{
    Diagnostic_Init();

    EXTI13_Init();

    Diagnostic_Run();

    CLI_Init();


    while (1)
    {
    	 if (Interrupt_ButtonEvent())
    	        {
    	            UART2_SendString("\r\n[EVENT] USER BUTTON PRESSED\r\n");
    	            UART2_SendString("> ");
    	        }
    	 CLI_Process();
    }
}
