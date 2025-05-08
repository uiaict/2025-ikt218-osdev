#ifndef PIT_H
#define PIT_H

#include "libc/stdint.h"

// PIT ports
#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40

// PIT frequency config
#define PIT_BASE_FREQUENCY 1193180
#define TARGET_FREQUENCY 1000 // 1000 Hz = 1ms per tick

#define TICKS_PER_MS 1

// Public functions
void init_pit();
void sleep_busy(uint32_t milliseconds);
void sleep_interrupt(uint32_t milliseconds);

#endif
