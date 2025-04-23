#ifndef PROGRAMMABLE_INTERVAL_TIMER_H
#define PROGRAMMABLE_INTERVAL_TIMER_H

#include "libc/stdint.h"

// PIT constants
#define PIT_BASE_FREQUENCY   1193180
#define TARGET_FREQUENCY     1000    // 1000 Hz = 1ms resolution

// PIT ports
#define PIT_CHANNEL0_PORT    0x40
#define PIT_CHANNEL1_PORT    0x41
#define PIT_CHANNEL2_PORT    0x42
#define PIT_COMMAND_PORT     0x43

// PIT command byte bits
#define PIT_CHANNEL0         0x00    // Channel 0 select
#define PIT_LOHI            0x30    // Access mode: lobyte/hibyte
#define PIT_MODE2           0x04    // Mode 2: rate generator

// Function declarations
void init_programmable_interval_timer(void);
void timer_handler(void);
uint32_t get_current_tick(void);
void sleep_busy(uint32_t milliseconds);
void sleep_interrupt(uint32_t milliseconds);

#endif // PROGRAMMABLE_INTERVAL_TIMER_H 