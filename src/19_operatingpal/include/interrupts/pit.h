// Header for PIT (Programmable Interval Timer) — based on template by Per-Arne Andersen
#ifndef PIT_H
#define PIT_H

#include "libc/stdint.h"
#include "libc/stdbool.h"

// PIT command and channel ports
#define PIT_CMD_PORT        0x43
#define PIT_CHANNEL0_PORT   0x40
#define PIT_CHANNEL1_PORT   0x41
#define PIT_CHANNEL2_PORT   0x42
#define PC_SPEAKER_PORT     0x61

// Default divisor for ~18.2 Hz tick rate (1193180 / 20000)
#define PIT_DEFAULT_DIVISOR 0x4E20

// PIC ports (for acknowledging PIT interrupts via IRQ0)
#define PIC1_CMD_PORT       0x20
#define PIC1_DATA_PORT      0x20
#define PIC_EOI             0x20

// Timer configuration constants
#define PIT_BASE_FREQUENCY  1193180
#define TARGET_FREQUENCY    1000                // Target frequency: 1000 Hz
#define DIVIDER             (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)
#define TICKS_PER_MS        (TARGET_FREQUENCY / TARGET_FREQUENCY)

// Initializes PIT with desired frequency and installs IRQ0 handler
void initPit();

// Sleeps using PIT interrupts (non-blocking)
void sleepInterrupt(uint32_t milliseconds);

// Sleeps using busy-waiting (blocking)
void sleepBusy(uint32_t milliseconds);

// PIT interrupt handler (called on each IRQ0)
void pitHandler();

#endif
