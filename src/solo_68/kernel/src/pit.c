#include "pit.h"
#include "irq.h"
#include "system.h"
#include "terminal.h"
#include <stdint.h>
#include "common.h"

static volatile uint32_t pit_ticks = 0;

static void pit_callback(registers_t regs) {
    (void)regs;
    pit_ticks++;
    outb(0x20, 0x20);
}

// Initialize the PIT to the target frequency
void init_pit() {
    uint32_t divisor = DIVIDER; // DIVIDER = PIT_BASE_FREQUENCY / TARGET_FREQUENCY

    // Send command byte
    outb(PIT_CMD_PORT, 0x36); // 0x36 = binary 00 11 0110
    // Channel 0, Access mode lobyte/hibyte, Operating mode 3 (square wave generator), Binary mode

    // Send divisor low byte first, then high byte
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(PIT_CHANNEL0_PORT, low);
    outb(PIT_CHANNEL0_PORT, high);

    // Install our timer handler (IRQ0)
    irq_install_handler(0, pit_callback);
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = pit_ticks;
    while ((pit_ticks - start_tick) < milliseconds) {
    }
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t start_tick = pit_ticks;
    while ((pit_ticks - start_tick) < milliseconds) {
        asm volatile ("hlt");
    }
}