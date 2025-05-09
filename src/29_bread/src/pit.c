#include <pit.h>
#include <libc/common.h>
#include <libc/irq.h>
#include <print.h>

// Global tick counter
static volatile uint32_t tick_count = 0;

// IRQ handler for timer interrupts
static void timer_callback(registers_t regs) {
    tick_count++;
}

// Get the current tick count
static uint32_t get_current_tick(void) {
    return tick_count;
}

void init_pit(void) {
    // Register our timer callback - use register_irq_handler instead of register_interrupt_handler
    register_irq_handler(0, timer_callback); // IRQ 0 is the timer IRQ
    
    // Set PIT to operate in mode 3 (square wave generator)
    outb(PIT_CMD_PORT, 0x36);
    
    // Set PIT reload value for 1000 Hz (1ms per tick)
    uint16_t divisor = DIVIDER;
    
    // Low byte, then high byte
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);
    
    printf("PIT initialized with divisor: %d\n", divisor);
}

void sleep_busy(uint32_t milliseconds) {
    uint32_t start_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;
    
    while (elapsed_ticks < ticks_to_wait) {
        while (get_current_tick() == start_tick + elapsed_ticks) {
            // Busy wait - this consumes CPU cycles
            asm volatile("pause");
        }
        elapsed_ticks++;
    }
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t current_tick = get_current_tick();
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = current_tick + ticks_to_wait;
    
    while (current_tick < end_ticks) {
        // Enable interrupts and halt the CPU until the next interrupt
        asm volatile(
            "sti\n"
            "hlt\n"
        );
        // Update current tick after waking from halt
        current_tick = get_current_tick();
    }
}