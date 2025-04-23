#include "song_player.h"
#include "libc/stdbool.h"
#include "terminal.h"
#include "pit.h"
#include "idt.h"



// play sound with given frequency
void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        return;
    }

    uint16_t divisor = (uint16_t)(PIT_BASE_FREQUENCY / frequency);

    // Set up the PIT
    outb(PIT_CMD_PORT, 0b10110110); 
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor >> 8));

    // Enable the speaker by setting bits 0 and 1
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, speaker_state | 0x03);
}

// Function to enable PC speaker
void enable_speaker() {
    // Read the current state of the PC speaker control register
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    
    // Enable the speaker by setting bits 0 and 1 to 1
    outb(PC_SPEAKER_PORT, speaker_state | 3);
}

// Function to disable PC speaker
void disable_speaker() {
    // Read the current state of the PC speaker control register
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    
    // Disable the speaker by clearing bits 0 and 1
    outb(PC_SPEAKER_PORT, speaker_state & 0xFC);
}

// Function to introduce a delay (in ms)
void delay(uint32_t duration) {
    sleep_interrupt(duration);
}

// Function to stop the sound
void stop_sound() {
    // Read the current state of the PC speaker control register
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    
    // Clear bit 1 (speaker data) while preserving bit 0 (speaker gate)
    // This keeps the speaker enabled but stops passing data to it
    outb(PC_SPEAKER_PORT, (speaker_state & ~0x02) | 0x01);
}
