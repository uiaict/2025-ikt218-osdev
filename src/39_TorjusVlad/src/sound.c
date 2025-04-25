#include "libc/stdint.h"
#include "libc/portio.h"
#include "sound.h"
#include "pit.h"  // for sleep_busy

#define PIT_CHANNEL2_PORT 0x42
#define PIT_COMMAND_PORT 0x43
#define PC_SPEAKER_PORT   0x61
#define PIT_BASE_FREQUENCY 1193180

void enable_speaker() {
       // Read the current state of the PC speaker control register
       uint8_t speaker_state = inb(PC_SPEAKER_PORT);
       /*
       Bit 0: Speaker gate
               0: Speaker disabled
               1: Speaker enabled
       Bit 1: Speaker data
               0: Data is not passed to the speaker
               1: Data is passed to the speaker
       */
       // Check if bits 0 and 1 are not set (0 means that the speaker is disabled)
       if (speaker_state != (speaker_state | 3)) {
           // If bits 0 and 1 are not set, enable the speaker by setting bits 0 and 1 to 1
           outb(PC_SPEAKER_PORT, speaker_state | 3);
       }
}

void disable_speaker() {
    // Turn off the PC speaker
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, speaker_state & 0xFC);
}

void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        disable_speaker();
        return;
    }
    enable_speaker();

    uint32_t divisor = (uint32_t)PIT_BASE_FREQUENCY / frequency;

    // Configure PIT channel 2: binary, mode 3, lobyte/hibyte
    outb(PIT_COMMAND_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF));
}

#define HZ_LOWER_LIMIT 20

void speaker_play_frequency(uint32_t frequency) {
    if ((frequency == 0) || (frequency < HZ_LOWER_LIMIT)) return;

    uint16_t divisor = PIT_BASE_FREQUENCY / frequency;

    outb(PIT_COMMAND_PORT, 0xB6); // Set PIT, channel 2, to square wave mode
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