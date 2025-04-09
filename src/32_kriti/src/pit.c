#include "pit.h"
#include "isr.h"
#include "kprint.h"

// Global variables for tracking ticks
volatile uint32_t tick_count = 0;
static bool pit_initialized = false;

// PIT interrupt handler
void pit_handler(uint8_t interrupt_num) {
    // Increment the tick counter
    tick_count++;
    
    // Send EOI to acknowledge the interrupt
    outb(PIC1_CMD_PORT, PIC_EOI);
}

// Get the current tick count
uint32_t get_current_tick() {
    return tick_count;
}

// Initialize the Programmable Interval Timer
void init_pit() {
    if (pit_initialized) {
        return;
    }
    
    // Calculate the divisor for the desired frequency
    uint16_t divisor = DIVIDER;
    
    // Set the PIT to operate in mode 3 (square wave generator)
    // 0x36 = 00110110
    // - 00 = channel 0
    // - 11 = access mode: low byte then high byte
    // - 011 = mode 3 (square wave generator)
    // - 0 = binary counting
    outb(PIT_CMD_PORT, 0x36);
    
    // Set the divisor (low byte first, then high byte)
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);         // Low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);  // High byte
    
    // Register the PIT handler to IRQ0 (interrupt 32)
    register_interrupt_handler(32, pit_handler);
    
    // Make sure IRQ0 is enabled in the PIC
    uint8_t current_mask = inb(PIC1_DATA_PORT);
    outb(PIC1_DATA_PORT, current_mask & ~0x01);  // Clear bit 0 to enable IRQ0
    
    pit_initialized = true;
    kprint("PIT initialized at ");
    kprint_dec(TARGET_FREQUENCY);
    kprint(" Hz\n");
}

// Sleep using interrupts for the specified number of milliseconds
void sleep_interrupt(uint32_t milliseconds) {
    // Make sure PIT is initialized
    if (!pit_initialized) {
        init_pit();
    }
    
    uint32_t current_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = current_tick + ticks_to_wait;
    
    kprint("Sleep interrupt: start=");
    kprint_dec(current_tick);
    kprint(", end=");
    kprint_dec(end_ticks);
    kprint("\n");
    
    while (current_tick < end_ticks) {
        // Enable interrupts and halt CPU until next interrupt
        __asm__ volatile ("sti; hlt");
        
        // Update current tick after interrupt
        current_tick = get_current_tick();
    }
    
    kprint("Sleep complete at tick ");
    kprint_dec(get_current_tick());
    kprint("\n");
}

// Sleep using busy waiting for the specified number of milliseconds
void sleep_busy(uint32_t milliseconds) {
    // Make sure PIT is initialized
    if (!pit_initialized) {
        init_pit();
    }
    
    uint32_t start_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;
    
    while (elapsed_ticks < ticks_to_wait) {
        // Busy wait until the tick changes
        while (get_current_tick() == start_tick + elapsed_ticks) {
            // Do nothing (busy wait)
            __asm__ volatile ("pause");  // Hint to CPU that this is a spin-wait loop
        }
        elapsed_ticks++;
    }
}

// Public function to get the current tick count
uint32_t get_tick_count(void) {
    return tick_count;
}