#include "kernel/pit.h"
#include "sys/io.h"
#include "libc/stdio.h"
#include "kernel/isr.h"

volatile uint32_t ticks = 0;
volatile uint32_t target_ticks = 0;
volatile bool sleep_active = false;

void timer_callback(registers_t regs) {
    ticks++;

    // Check if we're currently in a sleep cycle
    if (sleep_active && ticks >= target_ticks) {
        sleep_active = false;
    }
}

void init_pit() {
    register_interrupt_handler(IRQ0, timer_callback);
    outb(PIT_CMD_PORT, 0x36);

    // Send the divisor to the PIT
    outb(PIT_CHANNEL0_PORT, DIVIDER & 0xFF);         // Low byte
    outb(PIT_CHANNEL0_PORT, (DIVIDER >> 8) & 0xFF);  // High byte

    printf("PIT initialized at %u Hz\n", TARGET_FREQUENCY);
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t ms_ticks = milliseconds * TICKS_PER_MS;
    
    sleep_active = true;
    target_ticks = ticks + ms_ticks;
    
    while (sleep_active) {
        __asm__ volatile("hlt");
    }
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t ms_ticks = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = ticks + ms_ticks;
    
    while (ticks < end_ticks) {
        // Do nothing to burn CPU cycles
    }
}