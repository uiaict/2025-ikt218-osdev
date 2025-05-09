#include <kernel/interrupt/pit.h>
#include <driver/include/port_io.h>

////////////////////////////////////////
// PIT Tick Counter
////////////////////////////////////////

volatile uint32_t pit_ticks = 0;

////////////////////////////////////////
// PIT Initialization
////////////////////////////////////////

// Set PIT to mode 3 (square wave), using channel 0
void init_pit(void) {
    outb(PIT_COMMAND, 0x36);
    uint16_t divisor = (uint16_t)PIT_DIVISOR;
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));
}

////////////////////////////////////////
// Tick Access
////////////////////////////////////////

// Return current tick count
uint32_t get_current_tick(void) {
    return pit_ticks;
}

////////////////////////////////////////
// Delay Functions
////////////////////////////////////////

// Busy-wait using PIT ticks
void sleep_busy(uint32_t milliseconds) {
    uint32_t start = pit_ticks;
    while ((pit_ticks - start) < milliseconds) {
        asm volatile("pause");
    }
}

// Sleep with interrupts and halt
void sleep_interrupt(uint32_t milliseconds) {
    uint32_t start = pit_ticks;
    while ((pit_ticks - start) < milliseconds) {
        asm volatile("sti; hlt");
    }
}

////////////////////////////////////////
// PIT Frequency Control
////////////////////////////////////////

// Set PIT frequency (Hz)
void pit_set_frequency(uint32_t frequency) {
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);
    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));
}

////////////////////////////////////////
// PIT Interrupt Handler
////////////////////////////////////////

// Called from ISR to increment tick counter
void pit_handler(void) {
    pit_ticks++;
}
