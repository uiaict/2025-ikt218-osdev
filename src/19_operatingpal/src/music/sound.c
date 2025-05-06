#include "music/sound.h"
#include "interrupts/io.h"
#include "libc/stdint.h"
#include "interrupts/pit.h"

#define PIT_CHANNEL2_PORT 0x42
#define PIT_COMMAND_PORT  0x43
#define SPEAKER_CTRL_PORT 0x61
#define PIT_FREQUENCY     1193180

// Turns on the PC speaker
void enable_speaker() {
    uint8_t val = inb(SPEAKER_CTRL_PORT);
    outb(SPEAKER_CTRL_PORT, val | 0x03); // Set bits 0 and 1
}

// Turns off the PC speaker
void disable_speaker() {
    uint8_t val = inb(SPEAKER_CTRL_PORT);
    outb(SPEAKER_CTRL_PORT, val & 0xFC); // Clear bits 0 and 1
}

// Plays a tone at the given frequency
void play_sound(uint32_t freq) {
    if (freq == 0) return;

    uint32_t divisor = PIT_FREQUENCY / freq;

    outb(PIT_COMMAND_PORT, 0xB6); // Mode 3, channel 2

    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));        // Low byte
    outb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    enable_speaker();
}

// Stops the sound
void stop_sound() {
    disable_speaker();
}
