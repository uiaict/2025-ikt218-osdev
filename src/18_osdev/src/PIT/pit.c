#include "pit.h"
#include "../gdt/isr.h"
#include "libc/common.h"
#include "libc/stdint.h"

static volatile uint64_t tick = 0;


void pit_callback(registers_t regs) {
    tick++;
}

void init_pit() {
    register_interrupt_handler(IRQ0, &pit_callback);
    uint16_t divisor = DIVIDER;

    // Send command byte
    outb(PIT_CMD_PORT, 0x36); // Channel 0, Access mode: lobyte/hibyte, Mode 3 (square wave)

    // Send frequency divisor (low byte first, then high byte)
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));        // Low byte
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    tick = 0;
}

uint64_t get_tick() {
    return tick;
}


void sleep_interrupt(uint32_t milliseconds) {
    uint64_t end_tick = get_tick() + (milliseconds * TICKS_PER_MS);

    while (get_tick() < end_tick) {
        asm volatile("sti\n\thlt");
    }
}


void sleep_busy(uint32_t milliseconds) {
    uint64_t start_tick = get_tick();
    uint64_t ticks_to_wait = milliseconds * TICKS_PER_MS;

    while ((get_tick() - start_tick) < ticks_to_wait) {
        // do nothing (busy wait)
    }
}
