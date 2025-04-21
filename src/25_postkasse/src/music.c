#include "libc/stdint.h"
#include "libc/io.h"
#include "libc/music.h"
#include "libc/monitor.h"
#include "libc/pit.h"


#define SPEAKER_PORT 0x61

void enable_speaker() {
    uint8_t tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp | 3);
    monitor_write("Speaker enabled.\n");
}

void disable_speaker() {
    uint8_t tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp & 0xFC);
}

#define PIT_CHANNEL2    0x42     // Channel 2 data port
#define PIT_COMMAND     0x43     // Control port
#define MODE_3_SQUARE   0xB6     // Binary, mode 3 (square wave), lobyte/hibyte, channel 2

void play_sound(uint32_t frequency) {
    // 1. If frequency is 0, don't play anything
    if (frequency == 0) return;

    // 2. Calculate divisor
    uint16_t divisor = PIT_BASE_FREQUENCY / frequency;

    // 3. Send control word to PIT command register
    outb(PIT_COMMAND, MODE_3_SQUARE);

    // 4. Send frequency divisor to PIT channel 2
    outb(PIT_CHANNEL2, (uint8_t)(divisor & 0xFF));       // Low byte
    outb(PIT_CHANNEL2, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    // 5. Enable the speaker to route PIT channel 2 to it
    enable_speaker();
}

void stop_sound() {
    uint8_t tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp & 0xFC); // Clear speaker bits
}