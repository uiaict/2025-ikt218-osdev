#include "pit.h"
#include "printf.h"
#include "IDT.h"

// PIT constants
#define PIT_FREQUENCY 1193182  // Base frequency of the PIT in Hz
#define PIT_COMMAND   0x43     // Command port
#define PIT_CHANNEL0  0x40     // Channel 0 data port
#define PIT_CHANNEL1  0x41     // Channel 1 data port
#define PIT_CHANNEL2  0x42     // Channel 2 data port

// PIT configuration
#define PIT_BINARY    0x00     // Binary counter (0-65535)
#define PIT_BCD       0x01     // BCD counter (0-9999)
#define PIT_MODE0     0x00     // Interrupt on terminal count
#define PIT_MODE1     0x02     // Hardware retriggerable one-shot
#define PIT_MODE2     0x04     // Rate generator
#define PIT_MODE3     0x06     // Square wave generator
#define PIT_MODE4     0x08     // Software triggered strobe
#define PIT_MODE5     0x0A     // Hardware triggered strobe
#define PIT_LATCH     0x00     // Latch count value command
#define PIT_LOW       0x10     // Access low byte only
#define PIT_HIGH      0x20     // Access high byte only
#define PIT_BOTH      0x30     // Access low byte, then high byte
#define PIT_CHANNEL0S 0x00     // Select channel 0
#define PIT_CHANNEL1S 0x40     // Select channel 1
#define PIT_CHANNEL2S 0x80     // Select channel 2

// Store the current tick count
static volatile uint32_t current_tick = 0;

// Handler for PIT interrupt (IRQ0)
static void pit_irq_handler(registers_t* regs) {
    current_tick++;
}

// Get the current tick count
uint32_t get_current_tick() {
    return current_tick;
}

// Initialize the PIT
void init_pit() {
    // Set our PIT handler for IRQ0
    register_interrupt_handler(32, pit_irq_handler);
    
    // Calculate divisor
    uint16_t divisor = DIVIDER;
    
    // Send command byte: Channel 0, Access mode: lobyte/hibyte, Operating mode: rate generator
    outb(PIT_CMD_PORT, 0x36);
    
    // Send divisor (low byte then high byte)
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);         // Low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);  // High byte
    
    printf("PIT initialized at %d Hz\n", PIT_BASE_FREQUENCY / divisor);
}

// Sleep using busy waiting
void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;
    
    while (elapsed_ticks < ticks_to_wait) {
        // Busy wait until tick changes
        while (get_current_tick() == start_tick + elapsed_ticks) {
            // Do nothing - just burn CPU cycles
        }
        elapsed_ticks++;
    }
}

// Sleep using interrupts (more CPU efficient)
void sleep_interrupt(uint32_t milliseconds) {
    uint32_t current = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = current + ticks_to_wait;
    
    while (current < end_ticks) {
        // Enable interrupts and halt CPU until next interrupt
        asm volatile("sti; hlt");
        
        // Update current tick after waking up
        current = get_current_tick();
    }
}
