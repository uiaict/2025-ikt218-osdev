#include "pit/pit.h"
#include "libc/io.h"

volatile uint32_t pit_ticks = 0;

void pit_increment_tick() {
    pit_ticks++;
}

uint32_t get_current_tick() {
    return pit_ticks;
}

void init_pit(){
    uint32_t divisor = DIVIDER; // 1193180 / 1000
    outb(PIT_CMD_PORT, 0x36); // Set mode: channel 0, LSB+MSB, mode 3 (square wave)
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF); // Send LSB
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF); // Send MSB
   // outb(PC_SPEAKER_PORT, inb(PC_SPEAKER_PORT) | 3); // Enable speaker
}


void sleep_interrupt(uint32_t milliseconds){
    uint32_t current_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = current_tick + ticks_to_wait;

    while (current_tick < end_ticks) {
        __asm__ volatile ("sti"); // Enable interrupts
        __asm__ volatile ("hlt"); // Halt the CPU until the next interrupt
        current_tick = get_current_tick(); // Update current tick
    }
}


void sleep_busy(uint32_t milliseconds){
    uint32_t start_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;

    while (elapsed_ticks < ticks_to_wait) {
        while (get_current_tick() == start_tick + elapsed_ticks) {
            // Do nothing (busy wait)
        }
        elapsed_ticks++;
    }
}

uint32_t pit_get_tick() {
    return get_current_tick();
}

