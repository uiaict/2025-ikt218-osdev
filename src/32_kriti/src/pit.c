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

// Modified version of your PC Speaker functions in pit.c

// Set the PC speaker frequency using PIT channel 2
void set_pc_speaker_frequency(uint32_t frequency) {
    if (frequency == 0) {
        // For frequency 0, just disable the speaker
        disable_pc_speaker();
        return;
    }
    
    // Calculate divisor for PIT
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;
    
    // Ensure divisor is in range
    if (divisor > 65535) divisor = 65535;
    if (divisor < 1) divisor = 1;
    
    // Disable interrupts during this operation
    __asm__ volatile ("cli");
    
    // Configure PIT channel 2 for square wave generation
    // 10 = channel 2
    // 11 = access mode: low byte then high byte
    // 011 = mode 3 (square wave generator)
    // 0 = 16-bit binary
    outb(PIT_CMD_PORT, 0xB6);  // This is PIT_CHANNEL2_MODE3
    
    // Send divisor (low byte then high byte)
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);
    
    // Restore interrupts
    __asm__ volatile ("sti");
    
    kprint("PC Speaker frequency set to ");
    kprint_dec(frequency);
    kprint(" Hz\n");
}

// Enable the PC speaker
void enable_pc_speaker(void) {
    // Disable interrupts during port access
    __asm__ volatile ("cli");
    
    // Read current value
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    
    // Set both bits 0 and 1 to enable speaker
    // Bit 0: connects the PIT channel 2 to the speaker
    // Bit 1: enables the speaker gate
    outb(PC_SPEAKER_PORT, tmp | 0x03);  // This is PC_SPEAKER_ON_MASK (0x03)
    
    // Restore interrupts
    __asm__ volatile ("sti");
    
    kprint("PC Speaker enabled\n");
}

// Disable the PC speaker
void disable_pc_speaker(void) {
    // Disable interrupts during port access
    __asm__ volatile ("cli");
    
    // Read current value
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    
    // Clear bits 0 and 1 to disable speaker
    outb(PC_SPEAKER_PORT, tmp & 0xFC);  // This is PC_SPEAKER_OFF_MASK (0xFC)
    
    // Restore interrupts
    __asm__ volatile ("sti");
    
    kprint("PC Speaker disabled\n");
}

// Play a sound for the specified duration (blocking)
// This function is self-contained and more reliable
void beep_blocking(uint32_t frequency, uint32_t duration_ms) {
    kprint("Beeping at ");
    kprint_dec(frequency);
    kprint(" Hz for ");
    kprint_dec(duration_ms);
    kprint(" ms\n");
    
    if (frequency == 0) {
        // Just wait for the duration if frequency is 0 (rest)
        sleep_interrupt(duration_ms);
        return;
    }
    
    // Calculate divisor for PIT
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;
    if (divisor > 65535) divisor = 65535;
    if (divisor < 1) divisor = 1;
    
    // Disable interrupts during port access
    __asm__ volatile ("cli");
    
    // Configure PIT channel 2 for square wave generation
    outb(PIT_CMD_PORT, 0xB6);
    
    // Send divisor (low byte then high byte)
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);
    
    // Turn speaker on - read first to preserve other bits
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 0x03);
    
    // Restore interrupts
    __asm__ volatile ("sti");
    
    // Wait for the specified duration
    sleep_interrupt(duration_ms);
    
    // Disable interrupts during port access
    __asm__ volatile ("cli");
    
    // Turn speaker off
    tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC);
    
    // Restore interrupts
    __asm__ volatile ("sti");
    
    kprint("Beep complete\n");
}
// Add this function at the end of your pit.c file, 
// after your existing PC speaker functions

// Extremely simple and direct PC speaker test
void direct_speaker_test(void) {
    kprint("\n\n DIRECT SPEAKER TEST - SHOULD HEAR A LOUD 1kHz TONE \n\n");
    
    // Use direct port access for maximum compatibility
    // First ensure the speaker is off
    outb(0x61, inb(0x61) & 0xFC);
    
    // Configure PIT channel 2 for simple square wave (mode 3)
    outb(0x43, 0xB6);
    
    // Set a 1kHz tone (divisor = 1193180 / 1000 = 1193)
    uint16_t divisor = 1193;
    outb(0x42, divisor & 0xFF);
    outb(0x42, (divisor >> 8) & 0xFF);
    
    // Turn on the speaker (bits 0 and 1)
    outb(0x61, inb(0x61) | 0x03);
    
    kprint("Speaker should be ON - 1kHz tone\n");
    kprint("Waiting 3 seconds...\n");
    
    // Busy wait for 3 seconds
    for (volatile uint32_t i = 0; i < 150000000; i++) {
        if (i % 30000000 == 0) {
            kprint("*");
        }
    }
    
    // Turn off the speaker
    outb(0x61, inb(0x61) & 0xFC);
    
    kprint("\nSpeaker turned OFF\n");
}