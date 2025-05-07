#ifndef PIT_H
#define PIT_H

#include "libc/stdint.h"

// PIT Constants
#define PIT_BASE_FREQUENCY 1193180
#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL1_PORT 0x41
#define PIT_CHANNEL2_PORT 0x42
#define PIT_CMD_PORT      0x43
#define PC_SPEAKER_PORT   0x61

#define PIT_CMD_CHANNEL0  0x00
#define PIT_CMD_LOBYTE_HIBYTE 0x30
#define PIT_CMD_SQUARE_WAVE   0x06
#define PIT_CMD_BINARY        0x00

// PIC Constants
#define PIC1_CMD_PORT     0x20
#define PIC_EOI          0x20

// Timer frequency (100 Hz = 10ms per tick)
#define TARGET_FREQUENCY  100
#define TICKS_PER_MS     (TARGET_FREQUENCY / 1000)

// Function declarations
void init_pit(void);
void pit_handler(void);
uint32_t get_current_tick(void);

#endif