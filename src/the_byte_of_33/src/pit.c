#include "pit.h"
#include "io.h"
#include "interrupt.h"
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>


static volatile uint32_t pit_ticks = 0;

static void pit_callback(registers_t *regs) {
    pit_ticks++;
    outb(0x20, 0x20); // Send EOI to Master PIC
    (void)regs;
}

void init_pit() {
    outb(PIT_CMD_PORT, 0x34); // Set the PIT to mode 2 (rate generator)

    // Set the PIT to the desired frequency
    uint16_t divisor = (uint16_t)DIVIDER;
    
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF); // Low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF); // High byte

    // Register the PIT callback
    register_interrupt_handler(IRQ0, pit_callback); /////// IRQ0
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t target_ticks = pit_ticks + milliseconds;
    while (pit_ticks < target_ticks) {
        __asm__ volatile ("hlt"); // Halt CPU until next interrupt
    }
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t target_ticks = pit_ticks + milliseconds;
    while (pit_ticks < target_ticks) {
        // Busy wait
    }
}