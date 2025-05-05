#include "interrupts.h"
#include "pit.h"
#include "common.h"
#include "libc/system.h"
#include "monitor.h"
static uint32_t ticks = 0;


void test_pit_hardware() {
    printf("Testing PIT hardware directly...\n");
    
    // Read initial counter value
    outb(PIT_CMD_PORT, 0x00);  // Latch counter value
    uint8_t low = inb(PIT_CHANNEL0_PORT);
    uint8_t high = inb(PIT_CHANNEL0_PORT);
    uint16_t initial = (high << 8) | low;
    
    printf("Initial PIT counter: %d\n", initial);
    
    // Wait a bit using a busy loop
    for (volatile int i = 0; i < 1000000; i++) {}
    
    // Read final counter value
    outb(PIT_CMD_PORT, 0x00);  // Latch counter value
    low = inb(PIT_CHANNEL0_PORT);
    high = inb(PIT_CHANNEL0_PORT);
    uint16_t final = (high << 8) | low;
    
    printf("Final PIT counter: %d\n", final);
    
    if (initial != final) {
        printf("PIT HARDWARE IS RUNNING! Counter changed.\n");
    } else {
        printf("PIT HARDWARE NOT RUNNING! Counter didn't change.\n");
    }
}

// The PIT IRQ handler
void pit_irq_handler(registers_t* regs, void* context) {
    ticks++;
    // No need to send EOI - the irq_controller already does this
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

void test_timing_accuracy() {
    uint32_t start_ticks, end_ticks, elapsed_ticks;
    char buffer[16]; // Buffer for converting numbers to strings
   
    printf("Testing sleep_interrupt for 1000ms...\n");
   
    start_ticks = ticks;
    sleep_interrupt(1000);  // Should wait 1000ms
    end_ticks = ticks;
   
    elapsed_ticks = end_ticks - start_ticks;
   
    printf("Start ticks: %d\n", start_ticks);
    printf("End ticks: %d\n", end_ticks);
    printf("Elapsed ticks: %d\n", elapsed_ticks);
    printf("Expected ticks: 1000\n");
   
    if (elapsed_ticks >= 995 && elapsed_ticks <= 1005) {
        printf("Test PASSED! Timing is accurate.\n");
    } else {
        printf("Test FAILED! Timing is inaccurate.\n");
    }
}

void test_pit_direct() {
    printf("Direct PIT test\n");
    
    // Ensure interrupts are enabled
    asm volatile("sti");
    
    // Store the initial tick value
    uint32_t initial_ticks = ticks;
    printf("Initial ticks: %d\n", initial_ticks);
    
    // Try to force a small delay to see if ticks increment
    for (volatile uint32_t i = 0; i < 10000000; i++) {
        // Busy-wait
        if (i % 1000000 == 0) {
            printf("Busy wait iteration %d, ticks: %d\n", i/1000000, ticks);
        }
    }
    
    printf("After busy wait, ticks: %d\n", ticks);
    
    if (ticks > initial_ticks) {
        printf("PIT IS WORKING! Ticks increased by %d\n", ticks - initial_ticks);
    } else {
        printf("PIT NOT WORKING! Ticks did not increase.\n");
    }
}

void test_irq_registration() {
    printf("Testing IRQ registration...\n");
    
    // Re-register the handler with a different function to see if it works
    void test_irq_handler(registers_t* regs, void* context) {
        printf("TEST IRQ HANDLER CALLED!\n");
        outb(PIC1_CMD_PORT, PIC_EOI);
    }
    
    printf("Registering test IRQ handler...\n");
    register_irq_handler(IRQ0, test_irq_handler, NULL);
    
    // Enable interrupts and wait
    asm volatile("sti");
    
    printf("Waiting for test IRQ handler to be called...\n");
    // Wait a bit using busy wait
    for (volatile int i = 0; i < 10000000; i++) {
        if (i % 1000000 == 0) {
            printf("Still waiting... (iteration %d)\n", i / 1000000);
        }
    }
    
    printf("Test complete - did you see 'TEST IRQ HANDLER CALLED'?\n");
}