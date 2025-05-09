#include "pit.h"
#include "libc/system.h"
#include "libc/irq.h"

volatile uint32_t pit_ticks = 0;

void init_pit() {
    uint16_t divisor = DIVIDER;
    outb(PIT_CMD_PORT, 0x36);
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);
}

uint32_t get_current_tick() {
    return pit_ticks;
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;

    while (elapsed_ticks < ticks_to_wait) {
        while (get_current_tick() == start_tick + elapsed_ticks)
            ;
        elapsed_ticks++;
    }
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t current_tick = get_current_tick();
    uint32_t end_tick = current_tick + milliseconds * TICKS_PER_MS;

    while (get_current_tick() < end_tick) {
        __asm__ volatile ("sti; hlt");
    }
}

void pit_callback(registers_t* regs) {
    (void)regs;  // unused
    pit_ticks++;
}
