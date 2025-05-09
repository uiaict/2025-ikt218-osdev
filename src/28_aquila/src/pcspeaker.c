#include "pcspeaker.h"
#include "kernel/pit.h"
#include <libc/system.h>

void enable_speaker() {
    // Read state of port 0x61
    uint8_t current_state = inb(PC_SPEAKER_PORT);
    // Set bits 0 and 1 to enable the speaker
    if ((current_state & 0x03) != 0x03) { // Check if already enabled
        outb(PC_SPEAKER_PORT, current_state | 0x03);
    }
}

void disable_speaker() {
    // Read state of port 0x61
    uint8_t current_state = inb(PC_SPEAKER_PORT);

    // Clear bits 0 and 1
    outb(PC_SPEAKER_PORT, current_state & 0xFC);
}

void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        stop_sound(); 
        return;
    }

    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;

    if (divisor > 0xFFFF) divisor = 0xFFFF;
    if (divisor == 0) divisor = 1; // prevent div0
    outb(PIT_CMD_PORT, 0xB6);

    uint8_t low_byte = (uint8_t)(divisor & 0xFF);
    uint8_t high_byte = (uint8_t)((divisor >> 8) & 0xFF);
    outb(PIT_CHANNEL2_PORT, low_byte);
    outb(PIT_CHANNEL2_PORT, high_byte);

    enable_speaker();
}

void stop_sound() {
    // Read state of port 0x61
    uint8_t current_state = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, current_state & 0xFE);
}