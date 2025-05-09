// pit.h
#ifndef PIT_H
#define PIT_H

#include <stdint.h>
#include <stdbool.h>

// PIT (Programmable Interval Timer) ports
#define PIT_CMD_PORT        0x43
#define PIT_CHANNEL0_PORT   0x40
#define PIT_CHANNEL2_PORT   0x42

// PIC (Programmable Interrupt Controller) ports
#define PIC1_COMMAND        0x20
#define PIC_EOI             0x20

// PIT frequencies
#define PIT_BASE_FREQUENCY  1193180U
#define TARGET_FREQUENCY    1000U
#define DIVIDER             (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)

// Global tick counter updated by IRQ0
extern volatile uint32_t pit_ticks;

// Initialize the PIT
void init_pit(void);

// Get the current tick count
uint32_t get_current_tick(void);

// Sleeps by busy-waiting
void sleep_busy(uint32_t milliseconds);

// Sleeps by enabling interrupts and using hlt
void sleep_interrupt(uint32_t milliseconds);

#endif // PIT_H