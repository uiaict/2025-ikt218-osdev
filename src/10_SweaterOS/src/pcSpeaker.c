#include "pcSpeaker.h"
#include "display.h"
#include "programmableIntervalTimer.h"
#include "interruptHandler.h"

// Globale variabler for å unngå gjentatte I/O-operasjoner
static uint8_t speaker_state = 0;
static uint32_t current_frequency = 0;

/**
 * Enable the PC speaker
 * Sets bits 0 and 1 in the PC speaker control port to enable sound output
 * Optimalisert for å unngå unødvendige I/O-operasjoner
 */
void enable_speaker(void) {
    if (!(speaker_state & 0x03)) {
        uint8_t value = inb(0x61);
        value |= 0x03;
        outb(0x61, value);
        speaker_state = value;
    }
}

/**
 * Disable the PC speaker
 * Clears bits 0 and 1 in the PC speaker control port to disable sound output
 * Optimalisert for å unngå unødvendige I/O-operasjoner
 */
void disable_speaker(void) {
    // Alltid slå av høyttaleren når denne funksjonen kalles
    uint8_t value = inb(0x61);
    value &= ~0x03;  // Clear bits 0 and 1
    outb(0x61, value);
    speaker_state = value;
    current_frequency = 0;
}

/**
 * Calculate the PIT divisor for a given frequency
 * The PIT base frequency is 1.193180 MHz
 */
uint32_t calculate_pit_divisor(uint32_t frequency) {
    if (frequency == 0) return 0;
    return 1193180 / frequency;
}

/**
 * Play a sound at the specified frequency
 * This configures PIT channel 2 and enables the PC speaker
 * Optimalisert for lavere latens og raskere respons
 */
void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        disable_speaker();
        return;
    }
    
    // Fast path: If already playing the same frequency, do nothing
    if (frequency == current_frequency && (speaker_state & 0x03) == 0x03) {
        return;
    }
    
    current_frequency = frequency;

    // Calculate the PIT divisor for the desired frequency
    uint32_t divisor = calculate_pit_divisor(frequency);
    
    // Use direct I/O for maximum performance
    // Deaktiverer interrupts midlertidig for atomiske oppdateringer
    __asm__ volatile("cli");
    
    // Configure PIT channel 2 for square wave generation - more efficient command
    outb(0x43, 0xB6);  // Channel 2, square wave mode
    
    // Optimized approach - write both bytes with minimal delay between
    outb(0x42, divisor & 0xFF);        // Low byte
    outb(0x42, (divisor >> 8) & 0xFF); // High byte
    
    // Enable speaker directly without checking if already enabled
    // This slightly increases performance by eliminating branch and read
    outb(0x61, inb(0x61) | 0x03);
    speaker_state = inb(0x61);  // Update tracking variable accurately
    
    // Reaktiverer interrupts
    __asm__ volatile("sti");
}

/**
 * Stop playing sound
 * This disables the PC speaker output
 */
void stop_sound(void) {
    // Call disable_speaker directly to ensure sound is actually stopped
    disable_speaker();
} 