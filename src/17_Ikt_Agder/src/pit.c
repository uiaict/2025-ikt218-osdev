#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"
#include "libc/stdlib.h"
#include "libc/idt.h"
#include "libc/pit.h"
#include "libc/io.h"
#include "libc/interrupts.h"

volatile uint32_t pit_ticks = 0;  // Global tick counter


// Initialize the PIT
void init_pit(uint32_t frequency) {
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);

    // Send command byte
    outb(PIT_COMMAND, 0x36);

    // Send frequency divisor (low and high byte)
    outb(PIT_CHANNEL_0, divisor & 0xFF);
    outb(PIT_CHANNEL_0, (divisor >> 8) & 0xFF);

    // Register the IRQ handler for PIT (IRQ0)
    register_interrupt_handler(32, pit_handler);  // IRQ0 is mapped to interrupt 32
}

// Sleep using interrupts
void sleep_interrupt(uint32_t milliseconds) {
    uint32_t end_ticks = pit_ticks + (milliseconds * PIT_FREQUENCY / 1000);
    while (pit_ticks < end_ticks) {
        asm volatile("hlt");  // Halt CPU until next interrupt
    }
}

// Sleep using busy waiting
void sleep_busy(uint32_t milliseconds) {
    uint32_t end_ticks = pit_ticks + (milliseconds * PIT_FREQUENCY / 1000);
    while (pit_ticks < end_ticks);
}

void delay(uint32_t milliseconds) {
    uint32_t ticks = milliseconds * 1000; // Assuming PIT is running at 1000 Hz
    while (ticks > 0) {
        // Busy-wait loop
        ticks--;
    }
}