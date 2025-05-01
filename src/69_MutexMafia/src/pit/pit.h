#ifndef PIT_H
#define PIT_H

#include "libc/system.h" // Includes uint32_t etc.
#include "../idt/idt.h"
#include "../utils/utils.h"

#define IRQ0 0

// ===== PIT (Programmable Interval Timer) ports =====
#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_CHANNEL1_PORT 0x41
#define PIT_CHANNEL2_PORT 0x42
#define PC_SPEAKER_PORT 0x61

// ===== IRQ0 (timer interrupt) related macros =====
#define PIC1_CMD_PORT 0x20
#define PIC1_DATA_PORT 0x20
#define PIC_EOI 0x20 // End of interrupt

// ===== PIT frequency setup =====
#define PIT_BASE_FREQUENCY 1193180
#define TARGET_FREQUENCY 1000 // 1000 Hz for 1ms per tick
#define DIVIDER (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)
#define TICKS_PER_MS (TARGET_FREQUENCY / TARGET_FREQUENCY) // Always 1 with 1000 Hz

// ===== Interface =====
void init_pit();
void sleep_interrupt(uint32_t milliseconds);
void sleep_busy(uint32_t milliseconds);
uint32_t get_ticks();
void test_pit();

#endif // PIT_H
