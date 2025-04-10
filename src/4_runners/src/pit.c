#include "pit.h"
#include "libc/stdint.h"
#include "io.h"
#include "terminal.h" // for terminal_write
#include "isr.h"       // for int_to_string

static volatile uint32_t pit_ticks = 0;
void int_to_string(int value, char* buffer);  // Add this manually


void pit_handler() {
    pit_ticks++;
    outb(PIC1_CMD_PORT, PIC_EOI);  // Send End of Interrupt to PIC
}

uint32_t get_current_tick() {
    return pit_ticks;
}

void init_pit() {
    uint16_t divisor = DIVIDER;

    // Set PIT mode: channel 0, access low/high byte, mode 2 (rate generator)
    outb(PIT_CMD_PORT, 0x36);

    // Load the frequency divisor (low byte, then high byte)
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));

    pit_ticks = 0;

    // Output frequency message using terminal_write instead of printf
    terminal_write("PIT initialized at ");
    char freq_str[16];
    int_to_string(TARGET_FREQUENCY, freq_str);
    terminal_write(freq_str);
    terminal_write(" Hz\n");
}

// Busy-wait sleep — blocks CPU until the specified duration has passed
void sleep_busy(uint32_t milliseconds) {
    uint32_t start = get_current_tick();
    uint32_t wait_ticks = milliseconds * TICKS_PER_MS;

    uint32_t elapsed = 0;
    while (elapsed < wait_ticks) {
        while (get_current_tick() == start + elapsed); // busy-wait until tick increments
        elapsed++;
    }
}

// Interrupt-based sleep — puts CPU in low-power mode during wait
void sleep_interrupt(uint32_t milliseconds) {
    uint32_t end = get_current_tick() + (milliseconds * TICKS_PER_MS);
    while (get_current_tick() < end) {
        asm volatile("sti; hlt");
    }
}
