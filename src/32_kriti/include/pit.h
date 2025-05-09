#ifndef PIT_H
#define PIT_H

#include "libc/stdint.h"
#include "libc/stdbool.h"

// Ports
#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL1_PORT 0x41
#define PIT_CHANNEL2_PORT 0x42
#define PIT_CMD_PORT 0x43

// PIC Ports
#define PIC1_CMD_PORT  0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_CMD_PORT  0xA0
#define PIC2_DATA_PORT 0xA1
#define PIC_EOI        0x20

// PC Speaker Ports and Constants
#define PC_SPEAKER_PORT 0x61
#define PC_SPEAKER_ON_MASK 0x03
#define PC_SPEAKER_OFF_MASK 0xFC
#define PIT_BASE_FREQUENCY 1193180
#define PIT_CHANNEL2_MODE3 0xB6

// PIT Configuration
#define TARGET_FREQUENCY 100   // 100 Hz = 10ms per tick
#define DIVIDER (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)
#define TICKS_PER_MS (TARGET_FREQUENCY / 1000.0)

// Function prototypes
void init_pit(void);
void sleep_interrupt(uint32_t milliseconds);
void sleep_busy(uint32_t milliseconds);
uint32_t get_tick_count(void);

// PC Speaker function prototypes
void init_pc_speaker(void);
void enable_pc_speaker(void);
void disable_pc_speaker(void);
void set_pc_speaker_frequency(uint32_t frequency);
void beep(uint32_t frequency, uint32_t duration_ms);
void beep_blocking(uint32_t frequency, uint32_t duration_ms);
void direct_speaker_test(void);  // Added this prototype

#endif // PIT_H