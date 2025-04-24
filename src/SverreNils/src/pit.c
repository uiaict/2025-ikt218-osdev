#include "pit.h"
#include "libc/io.h"

#define PIT_FREQUENCY 1193180
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_IRQ 0
#define TARGET_HZ 1000

static volatile uint32_t tick = 0;

uint32_t get_tick() {
    return tick;
}

static void pit_callback() {
    tick++;
}

void init_pit() {
    uint32_t divisor = PIT_FREQUENCY / TARGET_HZ;

    outb(PIT_COMMAND_PORT, 0x36); // Mode 3, square wave
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));        // Low byte
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    // Registrer IRQ0 handler
    extern void irq_register_handler(int irq, void (*handler)());
    irq_register_handler(PIT_IRQ, pit_callback);
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start = get_tick();
    while ((get_tick() - start) < milliseconds) {
        // Busy wait
    }
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t end = get_tick() + milliseconds;
    while (get_tick() < end) {
        __asm__ volatile("sti\nhlt");
    }
}
