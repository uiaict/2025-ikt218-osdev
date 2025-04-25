#include "programmableIntervalTimer.h"
#include "display.h"
#include "interruptHandler.h"

// Timer tick counter
static volatile uint32_t tick_count = 0;

/**
 * Timer IRQ handler (IRQ0)
 * 
 * This function is called by the interrupt handler when a timer interrupt occurs.
 * It increments the timer tick counter.
 * Keep this function minimal and fast to reduce interrupt overhead!
 */
void timer_handler(void) {
    tick_count++;
    // Direct End of Interrupt to PIC1
    outb(PIC1_COMMAND, 0x20);
}

/**
 * Initialize the programmable interval timer
 * 
 * Configures the PIT to generate timer interrupts at specified frequency.
 * Uses Mode 3 (Square Wave Generator) for better performance and compatibility.
 */
void init_programmable_interval_timer(void) {
    display_write_color("Initializing Programmable Interval Timer...\n", COLOR_WHITE);
    
    // Calculate divisor based on desired frequency
    uint32_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;
    
    // If divisor is odd, make it even for Mode 3 (better performance)
    if (divisor & 1) {
        divisor &= ~1; // Clear lowest bit to make it even
    }
    
    // Make sure divisor is within valid range (1-65536)
    if (divisor < 2) divisor = 2;  // Mode 3 requires at least 2
    if (divisor > 65536) divisor = 65536;
    
    // Disable interrupts while configuring PIT
    __asm__ volatile("cli");
    
    // Configure PIT mode - use Mode 3 (Square Wave Generator)
    // Channel 0, lobyte/hibyte access, Mode 3, binary mode
    outb(PIT_COMMAND_PORT, PIT_CHANNEL0 | PIT_LOHI | PIT_MODE3);
    
    // Set frequency divisor
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);        // Low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF); // High byte
    
    // Enable IRQ0 (timer) in PIC
    uint8_t mask = inb(PIC1_DATA);
    mask &= ~(1 << 0);  // Clear bit 0 to enable timer interrupt
    outb(PIC1_DATA, mask);
    
    // Reset tick counter
    tick_count = 0;
    
    // Re-enable interrupts
    __asm__ volatile("sti");
    
    // Calculate actual frequency achieved
    uint32_t actual_freq = PIT_BASE_FREQUENCY / divisor;
    
    // Print status
    display_write_color("Timer initialized with frequency: ", COLOR_LIGHT_GREEN);
    display_write_decimal(actual_freq);
    display_write(" Hz (divisor: ");
    display_write_decimal(divisor);
    display_write(")\n");
    
    display_write_color("Timer interrupt (IRQ0) enabled\n", COLOR_LIGHT_GREEN);
}

/**
 * Get the current tick count
 * 
 * @return Current tick count since system start
 */
uint32_t get_current_tick(void) {
    return tick_count;
}

/**
 * Sleep for a specified number of milliseconds using busy waiting
 * (high CPU usage, actively checks the time)
 * 
 * @param milliseconds Number of milliseconds to sleep
 */
void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = tick_count;
    uint32_t wait_ticks = milliseconds;  // Since we configured for 1ms per tick
    
    while (tick_count - start_tick < wait_ticks) {
        __asm__ volatile("pause");  // CPU hint that we're spinning
    }
}

/**
 * Sleep for a specified number of milliseconds using interrupts
 * Modified to scale with the TARGET_FREQUENCY
 * 
 * @param milliseconds Number of milliseconds to sleep
 */
void sleep_interrupt(uint32_t milliseconds) {
    // For very short delays, use busy waiting instead for better precision
    if (milliseconds < 5) {
        sleep_busy(milliseconds);
        return;
    }

    // Check if interrupts are enabled
    uint32_t flags;
    __asm__ volatile("pushf\n\t"
                    "pop %0"
                    : "=r"(flags));
    
    if (!(flags & 0x200)) {  // If interrupts are disabled
        sleep_busy(milliseconds);  // Fall back to busy waiting
        return;
    }
    
    uint32_t start_tick = tick_count;
    // Convert milliseconds to ticks based on TARGET_FREQUENCY
    uint32_t wait_ticks = (milliseconds * TARGET_FREQUENCY) / 1000;
    if (wait_ticks == 0 && milliseconds > 0) {
        wait_ticks = 1; // At least wait one tick if needed
    }
    
    while (tick_count - start_tick < wait_ticks) {
        __asm__ volatile("pause");  // CPU hint that we're spinning (more efficient than nop)
        // Don't use hlt for audio timing as it can cause uneven timing intervals
    }
} 