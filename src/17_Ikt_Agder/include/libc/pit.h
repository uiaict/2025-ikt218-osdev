#ifndef PIT_H
#define PIT_H

#include <libc/stdint.h>

#define PIT_FREQUENCY 1193182  // Base frequency of PIT
#define PIT_CHANNEL_0 0x40
#define PIT_COMMAND 0x43

void init_pit(uint32_t frequency);  // Change int to uint32_t
void sleep_interrupt(uint32_t milliseconds);  // Change int to uint32_t
void sleep_busy(uint32_t milliseconds);  // Change int to uint32_t

#endif
