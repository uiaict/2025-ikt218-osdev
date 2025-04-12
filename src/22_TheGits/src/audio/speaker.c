#include "audio/speaker.h"
#include "pit/pit.h"
#include "libc/io.h"
#include "libc/scrn.h"

void enable_speaker() {
    uint8_t state = inb(PC_SPEAKER_PORT); // Read the current state of the speaker control port
    state |= 0b11; // Set the bits to enable the speaker. Both Bit 0 and Bit 1 are used for speaker control.
    outb(PC_SPEAKER_PORT, state); // Write the new state back to the control port
}

void disable_speaker() {
    uint8_t state = inb(PC_SPEAKER_PORT); // Read the current state of the speaker control port
    state &= ~0b11; // Clear both Bit 0 and Bit 1 to disable the speaker
    outb(PC_SPEAKER_PORT, state); // Write the new state back to the control port
}

void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        printf("play_sound: Rest (no sound)\n");
        return; // If frequency is 0, exit the function as this indicates no sound
    }

    printf("play_sound: freq = %d Hz\n", frequency);

    uint32_t divisor = PIT_BASE_FREQUENCY / frequency; // Calculate the divisor for the desired frequency

    // Set the PIT to the desired frequency
    outb(PIT_CMD_PORT, 0xB6); // Send control word to PIT control port
    
    // 0xB6 sets binary counting, mode 3 (square wave generator), and access mode (low/high byte)
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF); // Send the low byte of the divisor
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF); // Send the high byte of the divisor

    // Enable the speaker to start sound generation
    uint8_t state = inb(PC_SPEAKER_PORT); // Read the current state of the speaker control port
    state |= 0b11; // Set the bits to enable the speaker
    outb(PC_SPEAKER_PORT, state); // Write the new state back to the control port
}

void stop_sound() {
    uint8_t state = inb(PC_SPEAKER_PORT); // Read the current state of the speaker control port
    state &= ~0b10; // Clear only bit 1 to stop sound
    outb(PC_SPEAKER_PORT, state); // Write the new state back to the control port
}