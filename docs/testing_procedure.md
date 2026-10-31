# Testing Procedure and Verification Record

This document describes how to run the hardware checks implemented in the STM32 Hardware Diagnostics firmware and what each check verifies.

## Test environment

- **Board:** NUCLEO-L476RG / STM32L476RG
- **IDE:** STM32CubeIDE
- **Programming/debugging:** On-board ST-LINK
- **Serial terminal:** Tera Term or another serial terminal
- **UART settings:** 9600 baud, 8 data bits, no parity, 1 stop bit, no flow control
- **Measurement equipment:** Oscilloscope for the PWM output
- **External hardware:** I2C OLED module at address 0x3C and jumper wire for SPI loopback

## Before testing

1. Check the board and external wiring against [pin_connections.md](pin_connections.md).
2. Connect the NUCLEO-L476RG to the PC over USB.
3. Build the project in STM32CubeIDE and flash the firmware using ST-LINK.
4. Open the ST-LINK Virtual COM Port in the serial terminal with the settings above.
5. Confirm that the boot messages are visible.

A successful boot should report peripheral initialization and completion. Exact startup text may change as the firmware is developed.

## CLI reference

The current CLI is intentionally a **single-character command interface**.

| Key | Action | What it checks |
|---|---|---|
| `h` | Help | Prints the available commands |
| `s` | Startup self-test | Runs SPI, I2C, ADC and PWM checks |
| `p` | SPI test | Sends a byte and checks the loopback response |
| `i` | I2C test | Checks whether a device responds at address 0x3C |
| `a` | ADC test | Reads and prints an ADC conversion value |
| `w` | PWM status test | Reports that TIM2 PWM has been initialized |
| `d` | DMA test | Runs and verifies a DMA memory-to-memory transfer |
| `r` | Watchdog reset test | Starts the IWDG and intentionally waits for the MCU reset |

The CLI accepts one command character at a time. It is not a multi-word command parser.

## Individual test procedures

### 1. UART2 and CLI

1. Open the ST-LINK Virtual COM Port at 9600-8-N-1 with flow control disabled.
2. Reset or power-cycle the board.
3. Confirm that boot messages and the CLI banner appear.
4. Send `h` and confirm that the command list is printed.

**Pass indication:** readable boot and CLI messages appear in the serial terminal, and the help command prints the command list.

### 2. SPI1 loopback

1. Power down the board before attaching the jumper.
2. Connect **D11 / PA7 (MOSI)** to **D12 / PA6 (MISO)**.
3. Reconnect or reset the board.
4. Send `p` in the CLI.

The diagnostic transmits the test byte `0xA5` and compares the received byte with the transmitted byte.

**Pass indication:** the UART reports `[PASS] SPI1 LOOPBACK`.

This verifies the configured SPI transmit/receive path using a physical loopback jumper. It is not an external-slave interoperability test.

### 3. I2C1 and OLED detection

1. Connect OLED VCC to 3.3 V and GND to GND.
2. Connect SCL to D15 / PB8 and SDA to D14 / PB9.
3. Confirm that the bus has appropriate pull-ups.
4. Send `i` in the CLI.

The diagnostic checks for an I2C device at address **0x3C**.

**Pass indication:** the UART reports that the OLED was detected at 0x3C. The OLED display initialization and text output can also be visually checked.

### 4. ADC1 conversion

1. Ensure PA0 / A0 is connected to a valid analog input within the MCU's permitted voltage range.
2. Connect the signal source ground to board GND.
3. Send `a` in the CLI.

The firmware reads ADC1_IN5 on PA0 and prints the raw conversion result.

**Pass indication:** an ADC value is printed over UART. The raw reading depends on the voltage present at the pin; it is not expected to be a fixed number.

### 5. TIM2 PWM

1. Connect an oscilloscope probe to PA1 / A1.
2. Connect the oscilloscope ground to board GND.
3. Confirm that TIM2_CH2 is configured by the firmware.
4. Send `w` to view the firmware's PWM status message.
5. Inspect the output waveform on the oscilloscope.

**Pass indication:** a PWM waveform is visible on PA1. The CLI status message alone does not measure the waveform; use the oscilloscope for physical signal verification.

### 6. EXTI13 user-button interrupt

1. Keep the application running.
2. Press the on-board B1 user button.
3. Observe the UART terminal.

PC13 is configured as EXTI13 with a falling-edge trigger.

**Pass indication:** the UART reports `[EVENT] USER BUTTON PRESSED` after the button event.

### 7. DMA1 memory-to-memory transfer

1. Confirm that the board has booted and the CLI is available.
2. Send `d`.
3. Observe the UART result.

The test transfers a small byte array from a source buffer to a destination buffer using DMA1 Channel 1, then compares the destination with the source.

**Pass indication:** the UART reports `[PASS] DMA1 MEMORY TRANSFER`.

This test validates the configured memory-to-memory transfer and buffer comparison. It does not test DMA transfers to a peripheral.

### 8. Independent watchdog reset

**Warning:** This test intentionally resets the MCU. Any current application activity will be interrupted.

1. Save any work and make sure the board can be safely reset.
2. Send `r` in the CLI.
3. Observe the watchdog test message.
4. Wait for the MCU to reset and initialize again.

The test starts the Independent Watchdog (IWDG) and deliberately does not refresh it.

**Pass indication:** the MCU resets and the startup messages appear again in the serial terminal.

## Startup self-test

Send `s` to run the current startup self-test. The current `Diagnostic_Run()` implementation includes four checks:

1. SPI1 loopback
2. I2C1 device detection at 0x3C
3. ADC1 conversion
4. TIM2 PWM initialization status

The self-test prints a pass count and summary. DMA, EXTI button events and the watchdog reset are tested separately through their individual procedures; they are not included in the current four-test self-test count.

## Verification summary

| Feature | Verification method | Recorded development result |
|---|---|---|
| UART2 / CLI | Tera Term serial output and command input | Verified |
| SPI1 | MOSI-to-MISO loopback | Pass |
| I2C1 | OLED address detection | Detected at 0x3C |
| OLED | Text displayed on the module | Verified |
| ADC1 | Conversion value printed over UART | Verified |
| TIM2 PWM | Output inspected with oscilloscope | Verified |
| EXTI13 | Physical B1 button press | Event observed |
| DMA1 | Source/destination buffer comparison | Pass |
| IWDG | Observed MCU reset after watchdog timeout | Reset observed |

These results describe the development tests performed on the project hardware. They are not formal certification, stress testing, or production qualification.

## Debugging checklist

If a test does not pass:

1. Check that the relevant peripheral clock is enabled.
2. Check GPIO mode, alternate-function selection and pin mapping.
3. Check external wiring, common ground and logic voltage.
4. Inspect the relevant peripheral control and status registers in STM32CubeIDE's SFR view.
5. Compare the register state with the expected configuration and inspect the UART diagnostic output.
6. For PWM, SPI and I2C, use suitable measurement equipment or the relevant external device to verify the physical signal.
