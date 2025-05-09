#include "audio/speaker.h"
#include "pit/pit.h"
#include "libc/io.h"
#include "libc/scrn.h"

void enable_speaker() {
    uint8_t state = inb(PC_SPEAKER_PORT); // Read the current state of the speaker control port
    
    if(state != (state | 3)){
        outb(PC_SPEAKER_PORT, state | 3); // Set the bits to enable the speaker
    }
}

void disable_speaker() {
    uint8_t state = inb(PC_SPEAKER_PORT); // Read the current state of the speaker control port
    outb(PC_SPEAKER_PORT, state & ~3); // Write the new state back to the control port
}

void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        return; // If frequency is 0, exit the function as this indicates no sound
    }

    uint16_t divisor = (uint16_t)(PIT_BASE_FREQUENCY / frequency); // Calculate the divisor for the desired frequency

    // Set the PIT to the desired frequency
    outb(PIT_CMD_PORT, 0b10110110); // Send control word to PIT control port
    
    // 0xB6 sets binary counting, mode 3 (square wave generator), and access mode (low/high byte)
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF)); // Send the low byte of the divisor
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor >> 8)); // Send the high byte of the divisor

    // Enable the speaker to start sound generation
    uint8_t state = inb(PC_SPEAKER_PORT); // Read the current state of the speaker control port
    outb(PC_SPEAKER_PORT, state | 0x03); // Write the new state back to the control port
}

void stop_sound() {
    uint8_t state = inb(PC_SPEAKER_PORT); // Read the current state of the speaker control port
    outb(PC_SPEAKER_PORT, state & ~0x03); // Write the new state back to the control port
}
