#include "pit.h"
#include "interrupts.h"
#include "libc/io.h"

static uint32_t ticks = 0;


// Define the IRQ handler function
void pit_irq_handler(int int_no, void* context) {
    ticks++;
}


void init_pit() {

    
    // Register the IRQ handler
    register_irq_handler(IRQ0, pit_irq_handler, NULL);

    outb(PIT_CMD_PORT, 0x36);

    
    uint16_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;

    // Split up the divisor into upper and lower bytes
    uint8_t l_divisor = divisor & 0xFF;
    uint8_t h_divisor = (divisor >> 8) & 0xFF;
    

    // Send the frequency divisor
    outb(PIT_CHANNEL0_PORT, l_divisor);
    outb(PIT_CHANNEL0_PORT, h_divisor);
}


void sleep_interrupt(uint32_t milliseconds){

    uint32_t current_tick = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MILLISECOND;
    uint32_t end_ticks = current_tick + ticks_to_wait;

    while (current_tick < end_ticks) {
        // Enable interrupts (sti)
        asm volatile("sti");
        // Halt the CPU until the next interrupt (hlt)
        asm volatile("hlt");
        current_tick = ticks;
    }
}


void sleep_busy(uint32_t milliseconds){

    uint32_t start_tick = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MILLISECOND;
    uint32_t elapsed_ticks = 0;
    
    while (elapsed_ticks < ticks_to_wait) {
        while (ticks == start_tick + elapsed_ticks) {};
        elapsed_ticks++;
    }    
}