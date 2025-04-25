#include "pit.h"
#include "arch/isr.h"
#include "arch/irq.h"
#include "libc/io.h"

#define PIT_FREQUENCY     1193180
#define PIT_COMMAND_PORT  0x43
#define PIT_CHANNEL0_PORT 0x40
#define TARGET_HZ         1000 // 1 ms per tick
#define PIT_IRQ           0

static volatile uint32_t timer_ticks = 0;

uint32_t get_tick(void) {
    return timer_ticks;
}

static void pit_callback(struct registers* regs) {
    (void)regs;
    timer_ticks++;
}

void init_pit(void) {
    uint32_t divisor = PIT_FREQUENCY / TARGET_HZ;

    outb(PIT_COMMAND_PORT, 0x36);
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));

    irq_register_handler(PIT_IRQ, pit_callback);
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start = get_tick();
    while ((get_tick() - start) < milliseconds) {
        // Busy-wait
    }
}

void sleep_interrupt(uint32_t milliseconds) {
    if (milliseconds == 0) milliseconds = 1;
    uint32_t end = get_tick() + milliseconds;

    __asm__ volatile("sti"); // ✅ aktiver interrupts først!

    while (get_tick() < end) {
        __asm__ volatile("hlt");
    }

    __asm__ volatile("cli"); // (valgfritt) slå av igjen etterpå
}
