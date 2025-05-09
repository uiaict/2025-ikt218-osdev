// pit.c
#include "libc/pit.h"
#include "libc/isr.h"  // Changed from irq.h to isr.h to avoid conflicts
#include "libc/io.h"
#include "libc/stdint.h"

// IRQ0 is the timer interrupt
#define IRQ0 32  // IRQs start at interrupt vector 32

static volatile uint32_t tick = 0;  // Tracks system ticks

// Get current tick count
uint32_t get_tick() 
{
    return tick;
}

// Sleep using busy waiting (high CPU usage)
void sleep_busy(uint32_t milliseconds) 
{
    // Implementation follows assignment pseudocode
    uint32_t start_tick = get_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;
    
    while (elapsed_ticks < ticks_to_wait) {
        // Wait until we've reached the next tick
        while (get_tick() == start_tick + elapsed_ticks) {
            // Busy wait
        }
        elapsed_ticks++;
    }
}

// Sleep using interrupts (low CPU usage)
void sleep_interrupt(uint32_t milliseconds) 
{
    // Implementation follows assignment pseudocode
    uint32_t current_tick = get_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_tick = current_tick + ticks_to_wait;
    
    while (current_tick < end_tick) {
        // Enable interrupts and halt the CPU until the next interrupt
        asm volatile ("sti; hlt");
        
        // Update current tick after the interrupt
        current_tick = get_tick();
    }
}

// Timer IRQ handler - increments tick counter
// Made to match the isr_t function signature in isr.h
void timer_callback(registers_t regs) 
{
    tick++;
    outb(PIC1_CMD_PORT, PIC_EOI); // Send End Of Interrupt signal to PIC
}

// Initialize the PIT (Programmable Interval Timer)
void init_pit() 
{
    // Register our timer callback function using the function from isr.h
    register_interrupt_handler(32, timer_callback); // IRQ0 is interrupt 32

    // Calculate divisor for desired frequency
    uint32_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;

    // Send command byte: 
    // 0x36 = 00110110b
    // - Channel 0
    // - Access mode: lobyte/hibyte (11)
    // - Operating mode: square wave generator (011)
    // - Binary counting (0)
    outb(PIT_CMD_PORT, 0x36);
    
    // Send divisor in little-endian format (low byte, then high byte)
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);           // Low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);    // High byte
}