#include "speaker.h"
#include "port_io.h"

#define PC_SPEAKER_PORT 0x61
#define PIT_CHANNEL2    0x42
#define PIT_CMD_PORT    0x43
#define PIT_BASE_FREQ   1193180

void enable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 3);  // Set Bit 0 and Bit 1
}

void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT) & 0xFC; // Clear Bit 0 and 1
    outb(PC_SPEAKER_PORT, tmp);
}

void play_sound(uint32_t freq) {
    if (freq == 0) return;
    uint32_t divisor = PIT_BASE_FREQ / freq;

    outb(PIT_CMD_PORT, 0xB6); // Set up PIT: channel 2, mode 3, lobyte/hibyte
    outb(PIT_CHANNEL2, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2, (uint8_t)((divisor >> 8) & 0xFF));

    enable_speaker();
}

void stop_sound() {
    disable_speaker();
}
