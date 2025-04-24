#include "libc/stdint.h"
#include "libc/portio.h"
#include "sound.h"
#include "pit.h"  // for sleep_busy

#define PIT_CHANNEL2_PORT 0x42
#define PIT_COMMAND_PORT 0x43
#define PC_SPEAKER_PORT   0x61
#define PIT_BASE_FREQUENCY 1193180

void enable_speaker() {
    uint8_t val = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, val | 3); // set bits 0 and 1
}

void disable_speaker() {
    uint8_t val = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, val & ~3); // clear bits 0 and 1
}

void play_sound(uint32_t frequency) {
    if (frequency == 0) return; // silence

    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;

    // Configure PIT channel 2: binary, mode 3, lobyte/hibyte
    outb(PIT_COMMAND_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF));

    enable_speaker();
}

void stop_sound() {
    disable_speaker();
}
