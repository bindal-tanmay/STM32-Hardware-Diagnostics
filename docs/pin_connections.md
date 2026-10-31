# Pin Configuration and Hardware Connections

This document records the pin assignments and external wiring used by the STM32 Hardware Diagnostics project.

## Board and MCU

- **Development board:** NUCLEO-L476RG
- **Microcontroller:** STM32L476RG
- **Programming model:** Direct memory-mapped register access
- **GPIO logic level:** 3.3 V

## Peripheral pin map

| Function | MCU pin | NUCLEO-L476RG header | Configuration / use |
|---|---|---|---|
| USART2_TX | PA2 | D1 | USART2 alternate function AF7 |
| USART2_RX | PA3 | D0 | USART2 alternate function AF7 |
| SPI1_SCK | PA5 | D13 | SPI1 alternate function AF5 |
| SPI1_MISO | PA6 | D12 | SPI1 alternate function AF5 |
| SPI1_MOSI | PA7 | D11 | SPI1 alternate function AF5 |
| I2C1_SCL | PB8 | D15 | I2C1 alternate function AF4, open-drain |
| I2C1_SDA | PB9 | D14 | I2C1 alternate function AF4, open-drain |
| ADC1_IN5 | PA0 | A0 | Analog input |
| TIM2_CH2 | PA1 | A1 | PWM output, alternate function AF1 |
| EXTI13 | PC13 | On-board B1 user button | Falling-edge interrupt input |

Check the board pinout and silkscreen when wiring the board. Header labels and MCU pin names are both included above to make connections easier to identify.

## Connections

### UART2 through the ST-LINK Virtual COM Port

UART2 is routed through the board's ST-LINK Virtual COM Port. Connect the board to the PC over USB and select the ST-LINK virtual COM port in a serial terminal.

Serial settings:

- **Baud rate:** 9600
- **Data bits:** 8
- **Parity:** None
- **Stop bits:** 1
- **Flow control:** None

No external USB-to-UART adapter is required for this board-level VCP setup.

### SPI1 loopback test

For the SPI loopback test, connect:

| NUCLEO pin | Signal | Connect to |
|---|---|---|
| D11 / PA7 | SPI1_MOSI | D12 / PA6 |
| D12 / PA6 | SPI1_MISO | D11 / PA7 |
| D13 / PA5 | SPI1_SCK | No external connection needed for this loopback |

The firmware transmits a test byte and checks whether the same byte is received. This is a local loopback check; it does not validate communication with an external SPI peripheral.

**Pin-sharing note:** PA5 is also connected to the on-board LD2 LED. The project uses PA5 as SPI1_SCK during the SPI test. Do not run the PA5 GPIO LED demonstration at the same time as SPI1.

### I2C1 OLED

Connect the I2C OLED module as follows:

| OLED module | NUCLEO-L476RG |
|---|---|
| VCC | 3.3 V |
| GND | GND |
| SCL | D15 / PB8 |
| SDA | D14 / PB9 |

The OLED used during development responded at I2C address **0x3C**. The firmware's I2C diagnostic checks that address.

Use a 3.3 V-compatible module and ensure the I2C bus has suitable pull-up resistors. Do not connect the module to 5 V unless the module and its signal interface explicitly support it.

### ADC input

- **ADC channel:** ADC1_IN5
- **MCU pin:** PA0
- **Board header:** A0

The ADC test reads a single conversion from PA0 and prints the raw 12-bit conversion value over UART. For a controlled input test, use a 0–3.3 V signal referenced to board GND. Never apply a voltage outside the MCU pin's permitted electrical range.

### PWM output

- **Timer channel:** TIM2_CH2
- **MCU pin:** PA1
- **Board header:** A1

Connect an oscilloscope or logic-analyzer probe to PA1 and connect the instrument ground to board GND. The firmware configures PWM on this pin; the waveform was checked using an oscilloscope during development.

### User-button interrupt

The user-button interrupt uses **PC13 / EXTI13**, corresponding to the on-board B1 user button. The input is configured for a falling-edge event with an internal pull-up. Pressing the button should cause the firmware to report a button event over UART.

## Pin conflicts and practical notes

- **PA5 is shared:** it is used by both the on-board LD2 LED and SPI1_SCK. Do not expect the LED GPIO demonstration and SPI clock to operate independently on this pin at the same time.
- Do not drive a pin from two external sources simultaneously.
- Connect a common GND when using external signal sources or measurement instruments.
- Check the board pinout and voltage limits before connecting external hardware.
