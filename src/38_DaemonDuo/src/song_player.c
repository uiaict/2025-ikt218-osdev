#include "song_player.h"
#include "libc/stdbool.h"
#include "terminal.h"
#include "pit.h"
#include "idt.h"

// play sound with given frequency
void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        stop_sound();
        return;
    }

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
}

// Function to enable PC speaker
void enable_speaker() {
    // Read the current state of the PC speaker control register
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    
    // Prepare the speaker by setting bit 0 (connect PIT to speaker)
    // We don't set bit 1 yet, which would actually start the sound
    outb(PC_SPEAKER_PORT, tmp | 1);
}

// Function to disable PC speaker
void disable_speaker() {
    // Read the current state of the PC speaker control register
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    
    // Disable the speaker by clearing bits 0 and 1
    outb(PC_SPEAKER_PORT, tmp & 0xFC);
}

// Function to introduce a delay (in ms)
void delay(uint32_t duration) {
    sleep_interrupt(duration);
}

// Function to stop the sound
void stop_sound() {
    // Read the current state of the PC speaker control register
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    
    // Clear bit 1 (speaker data) while preserving other bits
    outb(PC_SPEAKER_PORT, tmp & ~0x02);
}
