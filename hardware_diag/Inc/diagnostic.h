#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <stdint.h>

void Diagnostic_Init(void);
void Diagnostic_Run(void);

uint8_t Diagnostic_TestSPI(void);
uint8_t Diagnostic_TestI2C(void);
uint8_t Diagnostic_TestADC(void);
uint8_t Diagnostic_TestPWM(void);
uint8_t Diagnostic_TestDMA(void);

#endif
