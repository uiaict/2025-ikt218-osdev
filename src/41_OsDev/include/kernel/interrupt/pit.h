#ifndef PIT_H
#define PIT_H

#include <libc/stdint.h>

////////////////////////////////////////
// PIT I/O Ports
////////////////////////////////////////

#define PIT_CHANNEL0         0x40
#define PIT_CHANNEL1         0x41
#define PIT_CHANNEL2         0x42
#define PIT_COMMAND          0x43

// Compatibility aliases
#define PIT_CHANNEL0_PORT    PIT_CHANNEL0
#define PIT_CHANNEL2_PORT    PIT_CHANNEL2
#define PIT_CMD_PORT         PIT_COMMAND

////////////////////////////////////////
// PIT Configuration
////////////////////////////////////////

#define PIT_FREQUENCY        1193180
#define PIT_BASE_FREQUENCY   PIT_FREQUENCY
#define PIT_DIVISOR          1000
#define DIVIDER              PIT_DIVISOR

// PC Speaker control port
#define PC_SPEAKER_PORT      0x61

////////////////////////////////////////
// PIT Interface
////////////////////////////////////////

// Initialize PIT with default frequency
void init_pit(void);

// Return current tick count
uint32_t get_current_tick(void);

// Sleep (busy loop, for tests)
void sleep_busy(uint32_t ms);

// Sleep using interrupts and halt
void sleep_interrupt(uint32_t ms);

// Set PIT frequency (Hz)
void pit_set_frequency(uint32_t frequency);

#endif // PIT_H
