#include "pit.h"
#include "common.h"
#include "terminal.h"
#include "interrupts.h"

// Count of timer ticks since boot
static volatile uint32_t tick_count = 0;

// IRQ0 handler - called on each timer tick
void pit_handler(registers_t* regs, void* data) {
    (void)regs;
    (void)data;
    
    tick_count++;
    
    // Show a tick counter once per second
    if (tick_count % 1000 == 0) {
        // Put counter in top-right corner
        uint16_t* vga = (uint16_t*)0xB8000;
        
        // Create string "Ticks: X" where X is the seconds
        char counter_str[12] = "Ticks:      ";
        uint32_t seconds = tick_count / 1000;
        int pos = 10;
        
        // Convert to string by repeatedly dividing by 10
        do {
            counter_str[pos--] = '0' + (seconds % 10);
            seconds /= 10;
        } while (seconds > 0 && pos >= 6);
        
        // Write to screen
        for (int i = 0; i < 12; i++) {
            vga[i + 80 - 12] = (0x0F << 8) | counter_str[i];
        }
    }
    
    // Send EOI to PIC
    outb(PIC1_CMD_PORT, PIC_EOI);
}

// Get current tick count
uint32_t get_current_tick(void) {
    return tick_count;
}

// Initialize PIT to run at 1000Hz
void init_pit() {
    // Register our timer handler for IRQ0
    register_irq_handler(0, &pit_handler, NULL);
    
    // Calculate and set frequency divisor
    uint32_t divisor = DIVIDER;
    
    // Send the command byte
    outb(PIT_CMD_PORT, 0x36); // Channel 0, square wave, binary
    
    // Send the frequency divisor
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);        // Low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF); // High byte
    
    // Make sure IRQ0 is enabled in the PIC
    uint8_t current_mask = inb(0x21);
    outb(0x21, current_mask & ~0x01); // Clear bit 0 to enable IRQ0
    
    terminal_writestring("PIT initialized at 1000 Hz\n");
}

// Sleep using busy-waiting (high CPU usage)
void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_tick = start_tick + ticks_to_wait;
    
    // Just keep checking the time until enough ticks have passed
    while (get_current_tick() < end_tick) {
        // Spin - do nothing
    }
}

// Sleep using interrupts (low CPU usage)
void sleep_interrupt(uint32_t milliseconds) {
    uint32_t start_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_tick = start_tick + ticks_to_wait;
    
    while (get_current_tick() < end_tick) {
        // Enable interrupts and halt the CPU
        // CPU will resume when an interrupt occurs
        asm volatile("sti");
        asm volatile("hlt");
    }
}