#include "pit.h"
#include "util.h"
#include "idt.h"
#include "vga.h"
#include "libc/stdint.h"

uint16_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;
volatile uint32_t tick = 0;

void pit_callback(struct InterruptRegisters* regs) {
    tick++;
}

void init_pit() {


    outPortB(PIT_CMD_PORT, 0x36);
    outPortB(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
    outPortB(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));

    irq_install_handler(0, pit_callback);
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start = tick;
    uint32_t wait_ticks = milliseconds * TICKS_PER_MS;

    while ((tick - start) < wait_ticks) {}
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t wait_ticks = milliseconds * TICKS_PER_MS;
    uint32_t end = tick + wait_ticks;

    while (tick < end) {
        asm volatile("sti; hlt");
    }
}
