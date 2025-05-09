#ifndef PIT_H
#define PIT_H

#include "libc/system.h"

#define PIT_CMD_PORT         0x43 // PIT Command Register port
#define PIT_CHANNEL0_PORT    0x40 // PIT Channel 0 Data port
#define PIT_CHANNEL1_PORT    0x41 // PIT Channel 1 Data port
#define PIT_CHANNEL2_PORT    0x42 // PIT Channel 2 Data port (often for PC speaker)

#define PC_SPEAKER_PORT      0x61 // Port to enable/disable PC speaker output


// Master PIC (PIC1)
#define PIC1_CMD_PORT        0x20 // PIC1 Command port
#define PIC1_DATA_PORT       0x21 // PIC1 Data port


#define PIC_EOI              0x20 // End-of-Interrupt command code (sent to PIC CMD_PORT)

#define PIT_BASE_FREQUENCY   1193180 // PIT input clock frequency in Hz (approx 1.193 MHz)
#define TARGET_FREQUENCY     1000 // 1000 Hz

#define PIT_DIVIDER_1KHZ     (PIT_BASE_FREQUENCY / TARGET_TIMER_FREQ_HZ) // Divisor for TARGET_TIMER_FREQ_HZ


#define TICKS_PER_MILLISECOND 1 // Since TARGET_TIMER_FREQ_HZ is 1000 Hz



// Initializes the PIT for a specific frequency
void init_pit();

void sleep_interrupt(uint32_t milliseconds);

// Pauses execution for 'milliseconds' using a busy-wait loop.
void sleep_busy(uint32_t milliseconds);

#endif