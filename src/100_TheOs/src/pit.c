#include "interrupts.h"
#include "pit.h"
#include "common.h"
#include "libc/system.h"
#include "monitor.h"
static uint32_t ticks = 0;

// Test the PIT for 10 seconds
// This test will check if the PIT is accurate within 1% of the expected time
void test_pit_10seconds(void) {
    uint32_t start_ticks, end_ticks, elapsed_ticks;
   
    printf("Starting 10-second PIT accuracy test...\n");
   
    // Ensure interrupts are enabled
    asm volatile("sti");
   
    // Record starting ticks
    start_ticks = ticks;
    printf("Start ticks: %d\n", start_ticks);
   
    // Sleep for 10 seconds (10000 ms)
    printf("Sleeping for 10 seconds...\n");
    sleep_interrupt(10000);
   
    // Record ending ticks
    end_ticks = ticks;
   
    // Calculate elapsed ticks
    elapsed_ticks = end_ticks - start_ticks;
   
    printf("End ticks: %d\n", end_ticks);
    printf("Elapsed ticks: %d\n", elapsed_ticks);
    printf("Expected ticks: 10000\n");
   
    // Calculate the actual accuracy
    uint32_t percent;
    uint32_t decimal;
    
    if (elapsed_ticks == 10000) {
        percent = 100;
        decimal = 0;
    } else if (elapsed_ticks > 10000) {
        uint32_t accuracy_x10000 = (10000 * 10000) / elapsed_ticks;
        percent = accuracy_x10000 / 100;
        decimal = accuracy_x10000 % 100;
    } else {
        uint32_t accuracy_x10000 = (elapsed_ticks * 10000) / 10000;
        percent = accuracy_x10000 / 100;
        decimal = accuracy_x10000 % 100;
    }
    if (decimal < 10) {
        printf("PIT timing accuracy: %d.0%d%%\n", percent, decimal);
    } else {
        printf("PIT timing accuracy: %d.%d%%\n", percent, decimal);
    }
    if (elapsed_ticks >= 9900 && elapsed_ticks <= 10100) {
        printf("TEST PASSED! Timing is accurate within 1%%.\n");
    } else if (elapsed_ticks >= 9500 && elapsed_ticks <= 10500) {
        printf("TEST FAILED! Timing is outside accurate range (>1%%).\n");
        printf("However, it's still within 5%% which may be acceptable.\n");
    } else {
        printf("TEST FAILED! Timing is significantly inaccurate (>5%%).\n");
    }
}

uint32_t get_uptime_seconds(void) {
    // Convert ticks to seconds based on your PIT frequency
    return ticks / TARGET_FREQUENCY;
}
// The PIT IRQ handler
void pit_irq_handler(registers_t* regs, void* context) {
    ticks++;
}

void init_pit() {
    // Register the IRQ handler
    register_irq_handler(0, pit_irq_handler, NULL);
    
    // Configure PIT channel 0 in rate generator mode (mode 2)
    outb(PIT_CMD_PORT, 0x34);  // 0x34 = 00110100b (channel 0, access mode low/high, mode 2, binary)
    
    // Set divisor for desired frequency
    uint16_t divisor = PIT_BASE_FREQUENCY / TARGET_FREQUENCY;
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);        // Low byte
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF); // High byte
    
    // Unmask IRQ0 (timer) in PIC
    outb(0x21, inb(0x21) & ~(1 << 0));  // Clear bit 0 in master PIC mask
    
    printf("PIT initialized with frequency %d Hz\n", TARGET_FREQUENCY);
}

void sleep_interrupt(uint32_t milliseconds) {
    uint32_t start_tick = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t end_ticks = start_tick + ticks_to_wait;
    
    printf("Sleep start: %d, wait for: %d, end at: %d\n", 
           start_tick, ticks_to_wait, end_ticks);
    
    // Debug counter
    uint32_t debug_counter = 0;
    
    while (ticks < end_ticks) {
        // Debug output every ~10000 iterations
        if (debug_counter % 10000 == 0) {
            printf("Current ticks: %d\n", ticks);
        }
        debug_counter++;
        
        // Enable interrupts and halt
        asm volatile("sti");
        asm volatile("hlt");
    }
    
    printf("Sleep complete, final ticks: %d\n", ticks);
}

void sleep_busy(uint32_t milliseconds){
    uint32_t start_tick = ticks;
    uint32_t ticks_to_wait = milliseconds * TICKS_PER_MS;
    uint32_t elapsed_ticks = 0;
    
    while (elapsed_ticks < ticks_to_wait) {
        while (ticks == start_tick + elapsed_ticks) {};
        elapsed_ticks++;
    }    
}