#include "pcSpeaker.h"
#include "display.h"
#include "programmableIntervalTimer.h"
#include "interruptHandler.h"

/**
 * Enable the PC speaker
 * Sets bits 0 and 1 in the PC speaker control port to enable sound output
 */
void enable_speaker(void) {
    uint8_t value = inb(0x61);
    outb(0x61, value | 0x03);
}

/**
 * Disable the PC speaker
 * Clears bits 0 and 1 in the PC speaker control port to disable sound output
 */
void disable_speaker(void) {
    uint8_t value = inb(0x61);
    outb(0x61, value & ~0x03);
}

/**
 * Calculate the PIT divisor for a given frequency
 * The PIT base frequency is 1.193180 MHz
 */
uint32_t calculate_pit_divisor(uint32_t frequency) {
    return 1193180 / frequency;
}

/**
 * Play a sound at the specified frequency
 * This configures PIT channel 2 and enables the PC speaker
 */
void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        disable_speaker();
        return;
    }

    // Calculate the PIT divisor for the desired frequency
    uint32_t divisor = calculate_pit_divisor(frequency);
    
    // Configure PIT channel 2 for square wave generation
    outb(0x43, 0xB6);  // Channel 2, square wave mode
    outb(0x42, divisor & 0xFF);
    outb(0x42, (divisor >> 8) & 0xFF);
    
    // Enable the speaker
    enable_speaker();
}

/**
 * Stop playing sound
 * This disables the PC speaker output
 */
void stop_sound(void) {
    disable_speaker();
} 