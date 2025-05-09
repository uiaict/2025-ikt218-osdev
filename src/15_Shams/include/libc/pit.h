#ifndef PIT_H
#define PIT_H

#include "libc/stdint.h"
#include "libc/io.h" // outb(), inb()

#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40

#define PIT_BASE_FREQUENCY 1193180
#define TARGET_FREQUENCY 1000
#define DIVIDER (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)
#define TICKS_PER_MS (TARGET_FREQUENCY / 1000)

void init_pit();
void sleep_interrupt(uint32_t milliseconds);
void sleep_busy(uint32_t milliseconds);
uint32_t get_current_tick();

#endif
