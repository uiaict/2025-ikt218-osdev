#include "music/sound.h"
#include "interrupts/io.h"
#include "libc/stdint.h"
#include "interrupts/pit.h"

#define PIT_CHANNEL2_PORT 0x42
#define PIT_COMMAND_PORT  0x43
#define SPEAKER_CTRL_PORT 0x61
#define PIT_FREQUENCY     1193180

void enable_speaker() {
    uint8_t val = inb(SPEAKER_CTRL_PORT);
    outb(SPEAKER_CTRL_PORT, val | 0x03); // Set bits 0 and 1
}

void disable_speaker() {
    uint8_t val = inb(SPEAKER_CTRL_PORT);
    outb(SPEAKER_CTRL_PORT, val & 0xFC); // Clear bits 0 and 1
}

void play_sound(uint32_t freq) {
    if (freq == 0) return;

    uint32_t divisor = PIT_FREQUENCY / freq;

    // Set PIT to mode 3 (square wave generator) on channel 2
    outb(PIT_COMMAND_PORT, 0xB6);

    // Send frequency divisor
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));        // Low byte
    outb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    enable_speaker();
}

void stop_sound() {
    disable_speaker();
}
