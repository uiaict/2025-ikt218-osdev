#include "interrupts/pit.h"

#include "libc/io.h"
#include "libc/stdint.h"

#define MAX_2_32 0xFFFFFFFF

static volatile uint32_t pit_ticks = 0;

void pit_tick() {
    pit_ticks++;
}

void pit_init() {
    uint16_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;

    outb(PIT_CMD_PORT, 0x34);
    
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF); // Low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF); // High byte
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t target_ticks = pit_ticks + milliseconds;

    while (pit_ticks < target_ticks) {
        __asm__ volatile ("hlt");
    }
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t target_ticks = pit_ticks + milliseconds;

    while (pit_ticks < target_ticks) {
        // Busy wait
    }
}