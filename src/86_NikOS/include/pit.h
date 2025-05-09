#ifndef PIT_H
#define PIT_H

#include "libc/system.h"
#include "libc/stdint.h"

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PC_SPEAKER 0x61
#define PIT_DEFAULT_DIVISOR 0x4E20

#define PIC1_CMD 0x20
#define PIC1_DATA 0x20
#define PIC_EOI		0x20

#define PIT_FREQUENCY 1193180
#define TARGET_FREQUENCY 1000
#define DIVIDER (PIT_FREQUENCY / TARGET_FREQUENCY)
#define TICKS_PER_MS (TARGET_FREQUENCY / 1000)

void pit_init();
void sleep_interrupt(uint32_t milliseconds);
void sleep_busy(uint32_t milliseconds);
uint32_t pit_get_ticks();
uint32_t pit_get_seconds();

#endif