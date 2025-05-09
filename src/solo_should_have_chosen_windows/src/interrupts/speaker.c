#include "interrupts/speaker.h"
#include "interrupts/pit.h"
#include "libc/io.h"

#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL2_PORT 0x42
#define PC_SPEAKER_PORT 0x61
#define PIT_BASE_FREQUENCY 1193180
#define HZ_LOWER_LIMIT 20

void speaker_play_frequency(uint32_t frequency) {
    if ((frequency == 0) || (frequency < HZ_LOWER_LIMIT)) return;

    uint16_t divisor = PIT_BASE_FREQUENCY / frequency;

    outb(PIT_CMD_PORT, 0xB6); // Set PIT, channel 2, to square wave mode
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF); // Set low byte
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF); // Set high byte

    uint8_t temp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, temp | 0x03); // Enable speaker
}

void speaker_stop() {
    uint8_t temp = inb(PC_SPEAKER_PORT) & 0xFC;
    outb(PC_SPEAKER_PORT, temp); // Disable speaker
}

void speaker_beep(uint32_t frequency, uint32_t duration_ms) {
    speaker_play_frequency(frequency);
    sleep_busy(duration_ms);
    speaker_stop();
}