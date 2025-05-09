#include "pit.h"
#include "port_io.h"
#include "terminal.h"
#include "irq.h"
#include <stdint.h>

volatile uint32_t timer_ticks = 0;

// PIT interrupt handler
void pit_callback(struct regs* r) {
    timer_ticks++;
}

// Initialize the PIT
void init_pit() {
    uint16_t divisor = DIVIDER;

    // Send command byte
    outb(PIT_CMD_PORT, 0x36);

    // Set divisor (low byte first, then high byte)
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));         // Low byte
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));  // High byte

    irq_install_handler(0, pit_callback); // Attach PIT interrupt to IRQ0
}

// Return the current tick count
uint32_t get_current_tick() {
    return timer_ticks;
}

// Busy-wait sleep function
void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;

    while (elapsed_ticks < ticks_to_wait) {
        while (get_current_tick() == start_tick + elapsed_ticks) {
            // Busy-wait: do nothing
        }
        elapsed_ticks++;
    }
}

// Sleep using interrupts (low CPU usage)
void sleep_interrupt(uint32_t milliseconds) {
    uint32_t current_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = current_tick + ticks_to_wait;

    while (get_current_tick() < end_ticks) {
        __asm__ __volatile__ ("sti\nhlt"); // Enable interrupts and halt until next interrupt
    }
}
