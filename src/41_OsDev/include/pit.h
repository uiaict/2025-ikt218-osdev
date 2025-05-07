#ifndef PIT_H
#define PIT_H

#include <stdint.h>
#include <stdbool.h>

// PIT ports
#define PIT_CMD_PORT        0x43
#define PIT_CHANNEL0_PORT   0x40

// PIC EOI for IRQ0
#define PIC1_COMMAND        0x20
#define PIC_EOI             0x20

// Timer frequency constants
#define PIT_BASE_FREQUENCY  1193180U
#define TARGET_FREQUENCY    1000U
#define DIVIDER             (PIT_BASE_FREQUENCY / TARGET_FREQUENCY)

// millisecond tick counter, bumped in IRQ0
extern volatile uint32_t pit_ticks;

// Initialize PIT to TARGET_FREQUENCY
void init_pit(void);

// Returns number of milliseconds since boot
uint32_t get_current_tick(void);

// Sleeps by busy-waiting
void sleep_busy(uint32_t milliseconds);

// Sleeps by halting until next interrupt
void sleep_interrupt(uint32_t milliseconds);

#endif // PIT_H
