#include "song_player.h"
#include "libc/stdbool.h"
#include "terminal.h"
#include "pit.h"
#include "idt.h"

// Flag to track if speaker is enabled
static bool speaker_enabled = false;

// play sound with given frequency
void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        stop_sound();
        return;
    }

    // Make sure we preserve the keyboard IRQ state
    uint8_t pic1_mask = inb(PIC1_DATA_PORT);
    bool keyboard_enabled = !(pic1_mask & (1 << 1));
    
    // Save current interrupt state and disable interrupts during configuration
    uint32_t eflags;
    asm volatile("pushf; pop %0" : "=r"(eflags));
    asm volatile("cli");

    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;
    
    // Set up PIT channel 2 for the tone
    outb(PIT_CMD_PORT, 0xB6); // 10110110 - Channel 2, lobyte/hibyte, mode 3 (square wave)
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF));

    // Read current speaker value
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    
    // Enable speaker by setting bits 0 and 1
    // Bit 0: Timer 2 gate to speaker enable
    // Bit 1: Speaker data enable
    outb(PC_SPEAKER_PORT, tmp | 3);
    speaker_enabled = true;
    
    // Restore interrupt state
    if (eflags & (1 << 9)) {  // Check if interrupts were enabled
        asm volatile("sti");
    }
    
    // Restore keyboard IRQ if it was enabled
    if (keyboard_enabled) {
        outb(PIC1_DATA_PORT, inb(PIC1_DATA_PORT) & ~(1 << 1));
    }
}

// Function to enable PC speaker
void enable_speaker() {
    // Save current interrupt state and disable interrupts during configuration
    uint32_t eflags;
    asm volatile("pushf; pop %0" : "=r"(eflags));
    asm volatile("cli");
    
    // Read the current state of the PC speaker control register
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    
    // Prepare the speaker by setting bit 0 (connect PIT to speaker)
    // We don't set bit 1 yet, which would actually start the sound
    outb(PC_SPEAKER_PORT, tmp | 1);
    speaker_enabled = true;
    
    // Restore interrupt state
    if (eflags & (1 << 9)) {  // Check if interrupts were enabled
        asm volatile("sti");
    }
}

// Function to disable PC speaker
void disable_speaker() {
    if (!speaker_enabled) {
        return;  // Already disabled
    }
    
    // Save current interrupt state and disable interrupts during configuration
    uint32_t eflags;
    asm volatile("pushf; pop %0" : "=r"(eflags));
    asm volatile("cli");
    
    // Read the current state of the PC speaker control register
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    
    // Disable the speaker by clearing bits 0 and 1
    outb(PC_SPEAKER_PORT, tmp & 0xFC);
    speaker_enabled = false;
    
    // Restore interrupt state
    if (eflags & (1 << 9)) {  // Check if interrupts were enabled
        asm volatile("sti");
    }
}

// Function to introduce a delay (in ms)
void delay(uint32_t duration) {
    sleep_interrupt(duration);
}

// Function to stop the sound
void stop_sound() {
    if (!speaker_enabled) {
        return;  // Already stopped
    }
    
    // Make sure we preserve the keyboard IRQ state
    uint8_t pic1_mask = inb(PIC1_DATA_PORT);
    bool keyboard_enabled = !(pic1_mask & (1 << 1));
    
    // Save current interrupt state and disable interrupts during configuration
    uint32_t eflags;
    asm volatile("pushf; pop %0" : "=r"(eflags));
    asm volatile("cli");
    
    // Read the current state of the PC speaker control register
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    
    // Clear bit 1 (speaker data) while preserving other bits
    outb(PC_SPEAKER_PORT, tmp & ~0x02);
    
    // Restore interrupt state
    if (eflags & (1 << 9)) {  // Check if interrupts were enabled
        asm volatile("sti");
    }
    
    // Restore keyboard IRQ if it was enabled
    if (keyboard_enabled) {
        outb(PIC1_DATA_PORT, inb(PIC1_DATA_PORT) & ~(1 << 1));
    }
}
