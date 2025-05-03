// src/arch/i386/pit.c

#include "pit.h"
#include "port_io.h"

static volatile uint32_t pit_ticks = 0;

void pit_callback() {
    pit_ticks++;
}

uint32_t get_current_tick() {
    return pit_ticks;
}
void sleep_busy(uint32_t milliseconds) {
    uint32_t start = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;

    while ((get_current_tick() - start) < ticks_to_wait) {
        // Busy wait
    }
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t start = get_current_tick();
    uint32_t end = start + (milliseconds * TICKS_PER_MS);

    while (get_current_tick() < end) {
        __asm__ volatile("hlt");
    }
}


void init_pit() {
    uint16_t divisor = PIT_DIVISOR;

    // Send command: Channel 0, Access mode: lobyte/hibyte, Mode 3 (square wave)
    outb(PIT_CMD_PORT, 0x36);

    // Send frequency divisor (low byte first, then high byte)
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));
}
