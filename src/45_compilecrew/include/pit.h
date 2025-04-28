#ifndef PIT_H
#define PIT_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/isr.h"  // <--- for registers_t

#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PC_SPEAKER_PORT 0x61

#define PIT_DEFAULT_DIVISOR 0x4E20

#define PIT_BASE_FREQUENCY 1193180
#define TARGET_FREQUENCY 1000
#define DIVIDER (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)
#define TICKS_PER_MS (TARGET_FREQUENCY / TARGET_FREQUENCY)

void init_pit();
void sleep_interrupt(uint32_t milliseconds);
void sleep_busy(uint32_t milliseconds);
void pit_callback(registers_t* regs);  // <--- must match definition!

#endif
