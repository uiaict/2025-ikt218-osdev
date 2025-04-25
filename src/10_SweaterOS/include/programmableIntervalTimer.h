#ifndef PROGRAMMABLE_INTERVAL_TIMER_H
#define PROGRAMMABLE_INTERVAL_TIMER_H

#include "libc/stdint.h"

// PIT constants
#define PIT_BASE_FREQUENCY   1193180  // 1.193182 MHz (3579545/3)
#define TARGET_FREQUENCY     1000     // Increased from 100 Hz to 1000 Hz for better audio performance

// PIT ports
#define PIT_CHANNEL0_PORT    0x40     // Channel 0 data port
#define PIT_CHANNEL1_PORT    0x41     // Channel 1 data port (not used)
#define PIT_CHANNEL2_PORT    0x42     // Channel 2 data port (PC speaker)
#define PIT_COMMAND_PORT     0x43     // Mode/Command register

// PIT command byte bits
#define PIT_CHANNEL0         0x00     // Channel 0 select
#define PIT_LOHI             0x30     // Access mode: lobyte/hibyte
#define PIT_MODE2            0x04     // Mode 2: rate generator
#define PIT_MODE3            0x06     // Mode 3: square wave generator

// Function declarations
void init_programmable_interval_timer(void);
void timer_handler(void);
uint32_t get_current_tick(void);
void sleep_busy(uint32_t milliseconds);
void sleep_interrupt(uint32_t milliseconds);

#endif // PROGRAMMABLE_INTERVAL_TIMER_H 