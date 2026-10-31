#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdint.h>

void Watchdog_Init(void);
void Watchdog_Refresh(void);
uint8_t Watchdog_TestReset(void);

#endif
