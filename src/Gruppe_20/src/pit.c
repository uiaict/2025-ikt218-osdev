#include "pit.h"
#include "interrupts.h"
#include "libc/stdio.h"
#include "libc/system.h"
#include "libc/print.h"
#include "libc/isr.h"
#include "io.h"

volatile uint32_t ticks = 0;

void pit_irq_handler(registers_t regs) {
    ticks++;
}

void init_pit() {
    register_interrupt_handler(IRQ0, pit_irq_handler);

    // Configure PIT to 1000Hz
    outb(PIT_CMD_PORT, 0x36);  // Channel 0, lobyte/hibyte, rate generator
    uint16_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t start_ticks = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    
    while ((ticks - start_ticks) < ticks_to_wait) {
        asm volatile("sti\n\t");
        asm volatile("hlt\n\t");
    }
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    while ((ticks - start_tick) < ticks_to_wait) {
        // Busy wait
    }
}


void stop_sound() {
    uint8_t tmp = inb(PC_SPEAKER_PORT) & 0xFC;
    outb(PC_SPEAKER_PORT, tmp);
}
