#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C1_Init(void);
uint8_t I2C1_DevicePresent(uint8_t address);

uint8_t I2C1_Write(uint8_t address, const uint8_t *data, uint8_t length);

#endif
