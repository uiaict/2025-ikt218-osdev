#include "pit.h"
#include "idt.h"
#include "terminal.h"

// Global variables
volatile uint32_t tick_count = 0;
volatile bool sleep_active = false;
volatile uint32_t sleep_ticks = 0;

// Initialize the PIT (Programmable Interval Timer)
void init_pit() {
    uint32_t divisor = DIVIDER;

    // Set command byte: channel 0, access mode: lobyte/hibyte, operating mode: rate generator
    outb(PIT_CMD_PORT, 0x36);

    // Set frequency divisor
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);           // Low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);    // High byte
    
    // Initialize channel 2 for PC speaker (but keep it off initially)
    outb(PIT_CMD_PORT, 0xB6);  // 10110110 - Channel 2, lobyte/hibyte, mode 3 (square wave)
    outb(PIT_CHANNEL2_PORT, 0xFF);
    outb(PIT_CHANNEL2_PORT, 0xFF);
    
    // Make sure the speaker is disabled initially
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC);  // Clear bits 0 and 1
    
    writeline("PIT initialized\n");
}

// PIT tick handler - called by IRQ0 handler
void pit_tick() {
    tick_count++;

    // Check if we're in a sleep state
    if (sleep_active) {
        if (--sleep_ticks == 0) {
            sleep_active = false;
        }
    }
}

// Reset channel 0 to timer mode after using it for sound
void reset_pit_timer() {
    uint32_t divisor = DIVIDER;
    
    // Set command byte: channel 0, access mode: lobyte/hibyte, operating mode: rate generator
    outb(PIT_CMD_PORT, 0x36);
    
    // Set frequency divisor
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);           // Low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);    // High byte
}

// Sleep using interrupts (more efficient)
void sleep_interrupt(uint32_t milliseconds) {
    // Convert milliseconds to PIT ticks
    sleep_ticks = milliseconds * TICKS_PER_MS;
    
    // Enter sleep state
    sleep_active = true;
    
    // Wait until sleep_active becomes false (set by the PIT tick handler)
    while (sleep_active) {
        __asm__ __volatile__("hlt");
    }
}

// Sleep using busy waiting (less efficient but simpler)
void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = tick_count;
    uint32_t wait_ticks = milliseconds * TICKS_PER_MS;
    
    while (tick_count - start_tick < wait_ticks) {
        __asm__ __volatile__("pause");  // CPU hint for spin-wait loop
    }
}
