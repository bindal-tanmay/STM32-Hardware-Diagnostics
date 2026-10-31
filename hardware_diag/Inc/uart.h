#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART2_Init(void);
void UART2_SendChar(char c);
void UART2_SendString(const char *str);
char UART2_ReceiveChar(void);
int UART2_ReceiveCharNonBlocking(char *c);
#endif
