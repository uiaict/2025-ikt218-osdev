#ifndef PIT_H
#define PIT_H

#include "libc/stdint.h"

// PIT Ports
#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL2_PORT 0x42
#define PIT_CMD_PORT      0x43
#define PC_SPEAKER_PORT   0x61

// PIT Frequencies
#define PIT_BASE_FREQUENCY 1193180
#define TARGET_FREQUENCY   1000
#define DIVIDER           (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)
#define TICKS_PER_MS      1

// PIC ports
#define PIC1_CMD_PORT  0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_CMD_PORT  0xA0
#define PIC2_DATA_PORT 0xA1

// PIC commands
#define PIC_EOI 0x20 

// Function declarations
void init_pit(void);
void pit_set_speaker_freq(uint32_t frequency);
void sleep_busy(uint32_t milliseconds);
void sleep_interrupt(uint32_t milliseconds);
uint32_t get_current_tick(void);

#endif