#include "libc/stdint.h"
#include "libc/stdbool.h"
#include "ports.h"
#include "pit.h"
#include "rng.h"
#include "terminal.h"
#include "keyboard.h"
#include "snake.h"

static uint32_t pit_ticks = 0;
static bool pit_initialized = false;

void pit_init() {
    uint32_t divisor = PIT_FREQUENCY / TARGET_FREQUENCY;

    outb(PIT_COMMAND, 0x36);

    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));

    pit_initialized = true;
}

void pit_handler() {
    // RNG BISH
    uint32_t _ = rand();

    pit_ticks++;

    if (is_playing_snake()) {
        snake_update();
    }
}

uint32_t pit_get_ticks() {
    return pit_ticks;
}

uint32_t pit_get_seconds() {
    return pit_ticks / TARGET_FREQUENCY;
}

void sleep_interrupt(uint32_t milliseconds) {
    if (!pit_initialized) pit_init();

    uint32_t end_ticks = pit_ticks + (milliseconds * TICKS_PER_MS);
    while (pit_ticks < end_ticks) {
        asm volatile("sti");
        asm volatile("hlt");
    }
}

void sleep_busy(uint32_t milliseconds) {
    if (!pit_initialized) pit_init();

    uint32_t end_ticks = pit_ticks + (milliseconds * TICKS_PER_MS);
    while (pit_ticks < end_ticks) {
        asm volatile("nop");
    }
}