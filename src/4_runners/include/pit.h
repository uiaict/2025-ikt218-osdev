#ifndef PIT_H
#define PIT_H

#include "libc/stdint.h"
#include "libc/stdbool.h"

// PIT (Programmable Interval Timer) I/O ports
#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL1_PORT 0x41
#define PIT_CHANNEL2_PORT 0x42
#define PC_SPEAKER_PORT 0x61

// PIC (Programmable Interrupt Controller) ports
#define PIC1_CMD_PORT  0x20
#define PIC1_DATA_PORT 0x21
#define PIC_EOI        0x20  // End of Interrupt

// PIT frequency
#define PIT_BASE_FREQUENCY 1193180
#define TARGET_FREQUENCY   1000  // 1000Hz = 1ms per tick
#define DIVIDER            (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)
#define TICKS_PER_MS       (TARGET_FREQUENCY / 1000)

void init_pit(void);
void sleep_busy(uint32_t milliseconds);
void sleep_interrupt(uint32_t milliseconds);
void pit_handler(void);
uint32_t get_current_tick(void);

#endif
