#ifndef PIT_H
#define PIT_H

#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/common.h>
#include <libc/irq.h>
#include <print.h>

// PIT (Programmable Interval Timer) related macros
#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL1_PORT 0x41
#define PIT_CHANNEL2_PORT 0x42
#define PC_SPEAKER_PORT 0x61
#define PIT_DEFAULT_DIVISOR 0x4E20 // 20000, which gives about 18.2 Hz (1193180 / 20000)

// IRQ0 related macros
#define PIC1_CMD_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_CMD_PORT 0xA0
#define PIC2_DATA_PORT 0xA1

#define PIC_EOI     0x20        /* End-of-interrupt command code */
 
// Custom sleep function constants
#define PIT_BASE_FREQUENCY 1193180
#define TARGET_FREQUENCY 1000 // 1000 Hz
#define DIVIDER (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)
#define TICKS_PER_MS 1 // Each tick is 1ms (1000Hz)

void init_pit(void);
void sleep_interrupt(uint32_t milliseconds);
void sleep_busy(uint32_t milliseconds);

#endif