#include "kernel/pit.h"

volatile uint32_t tick = 0;

#define MS_PER_TICK 1 
#define ms_to_ticks(ms) ((ms) / MS_PER_TICK)

void init_pit() {
    uint32_t divisor = DIVIDER; // set divisor for 1000 Hz
    outb(PIT_CMD_PORT, 0x36); 
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF)); // send LSB
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // send MSB
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = tick;
    uint32_t target_ticks = ms_to_ticks(milliseconds);

    while (tick < start_tick + target_ticks) {
        // busy-wait
    }
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t end_tick = tick + ms_to_ticks(milliseconds);
    while (tick < end_tick) {
        asm volatile("sti");  // enable interrupts
        asm volatile("hlt");  // halt until next interrupt
    }
}