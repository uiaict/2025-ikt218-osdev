#include <pit.h>
#include <utils.h>
#include <interrupts.h>
#include <libc/stdint.h>

static uint32_t ticks = 0;

void timer_handler(void) {
    ticks++;
    outb(0x20, 0x20); 
}

void init_pit() {
    register_irq_handlers(IRQ0, timer_handler); // Register the timer handler for IRQ0
    outb(PIT_CMD_PORT, 0x36);                       // Set the PIT to mode 3 (square wave generator)
    uint8_t high_div = (uint8_t)(DIVIDER >> 8);     // Get the high byte of the divisor
    uint8_t low_div = (uint8_t)(DIVIDER);           // Get the low byte of the divisor

    outb(PIT_CHANNEL0_PORT, low_div);               // Send the low byte to channel 0
    outb(PIT_CHANNEL0_PORT, high_div);              // Send the high byte to channel 0
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;
    while (elapsed_ticks < ticks_to_wait) {
        while (ticks == start_tick + elapsed_ticks) {}
        elapsed_ticks++;
    }
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t current_tick = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = ticks + ticks_to_wait;
    while (ticks < end_ticks) {
        asm volatile("sti");
        asm volatile("hlt");
        current_tick = ticks;
    }
}

uint32_t get_current_ticks() {
    return ticks;
}