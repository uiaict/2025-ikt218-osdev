#include "../PIT/pit.h"
#include <system.h>


// PIT tick counter, will be incremented by the timer interrupt handler
volatile uint32_t tick_count = 0;

// Function to handle timer interrupt
void timer_handler() {
    tick_count++;
    
    // Send EOI (End of Interrupt) to the PIC
    outb(PIC1_CMD_PORT, PIC_EOI);
}

// Function to get the current tick count
uint32_t get_current_tick() {
    return tick_count;
}

// Initialize the Programmable Interval Timer
void init_pit() {
    // Calculate the divisor for the desired frequency
    uint16_t divisor = DIVIDER;
    
    // Set PIT command byte - channel 0, access mode: lobyte/hibyte, mode 3 (square wave)
    outb(PIT_CMD_PORT, 0x36);
    
    // Set the divisor - low byte
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);
    
    // Set the divisor - high byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);
    
    // Install the timer handler to IRQ0
    // This will require you to have a function to register IRQ handlers 
    irq_install_handler(0, timer_handler);
    
    printf("PIT initialized at %u Hz\n", TARGET_FREQUENCY);
}

// Sleep using busy waiting
void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;
    
    while (elapsed_ticks < ticks_to_wait) {
        // Busy wait until we see a tick change
        while (get_current_tick() == start_tick + elapsed_ticks) {
            // Do nothing - busy waiting
        }
        elapsed_ticks++;
    }
}

// Sleep using interrupts
void sleep_interrupt(uint32_t milliseconds) {
    uint32_t current_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = current_tick + ticks_to_wait;
    
    while (current_tick < end_ticks) {
        // Enable interrupts and halt the CPU until the next interrupt
        asm volatile("sti");
        asm volatile("hlt");
        
        // Update current tick after waking up
        current_tick = get_current_tick();
    }
}