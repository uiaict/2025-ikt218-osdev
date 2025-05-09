#include "pit.h"
#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "libc/stdio.h"


// Assembly I/O functions
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Global tick counter
volatile uint32_t pit_ticks = 0;

// Called from IRQ0 handler to increment tick count
void pit_tick() {
    pit_ticks++;
}

// Initialize the PIT to fire IRQ0 at 1000 Hz
void init_pit() {
    uint16_t divisor = DIVIDER; // Typically 1193180 / 1000 = 1193

    // Send command byte:
    // - Channel 0 (00)
    // - Access mode: lobyte/hibyte (11)
    // - Operating mode: Mode 2 (Rate generator) (010)
    // - Binary mode (0)
    outb(PIT_CMD_PORT, 0x36); // 00 11 010 0 = 0x36

    // Send divisor (low byte first, then high byte)
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));       // Low byte
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    pit_ticks = 0;

    printf("PIT initialized to %u Hz\n", TARGET_FREQUENCY);
}

// Sleep using PIT interrupts
void sleep_interrupt(uint32_t milliseconds) {
    uint32_t target_ticks = pit_ticks + milliseconds * TICKS_PER_MS;
    while (pit_ticks < target_ticks) {
        // Halt CPU until next interrupt to save power
        __asm__ volatile ("hlt");
    }
}

// Sleep using busy waiting (less efficient)
void sleep_busy(uint32_t milliseconds) {
    uint32_t start = pit_ticks;
    while (pit_ticks < start + milliseconds * TICKS_PER_MS);
}
