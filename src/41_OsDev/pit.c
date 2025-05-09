//pit.c
#include <stdint.h>
#include "pit.h"
#include "port_io.h"

volatile uint32_t pit_ticks = 0;

void init_pit(void) {
    // Channel 0, lobyte/hibyte, mode 3 (square wave)
    outb(PIT_CMD_PORT, 0x36);

    uint16_t divisor = (uint16_t)DIVIDER;
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));
}

uint32_t get_current_tick(void) {
    return pit_ticks;
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start = pit_ticks;
    while ((pit_ticks - start) < milliseconds) {
        // busy‐wait
    }
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t start = pit_ticks;
    while ((pit_ticks - start) < milliseconds) {
        asm volatile("sti; hlt");
    }
}
