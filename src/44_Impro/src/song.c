#include "libc/stdint.h"

#include "pit.h"
#include "song/song.h"
#include "util.h"

#define PIT_FREQUENCY 1193180
#define PIT_CHANNEL2_PORT 0x42
#define PIT_CMD_PORT 0x43
#define SPEAKER_CTRL_PORT 0x61




void enable_speaker() {
    uint8_t tmp = inPortB(SPEAKER_CTRL_PORT);
    if (tmp != (tmp | 3)) {
        outPortB(SPEAKER_CTRL_PORT, tmp | 3);
    }
}

void disable_speaker() {
    uint8_t tmp = inPortB(SPEAKER_CTRL_PORT);
    outPortB(SPEAKER_CTRL_PORT, tmp & ~3);
}

void play_sound(uint32_t freq) {
    if (freq == 0) return;

    uint32_t divisor = PIT_FREQUENCY / freq;

    outPortB(PIT_CMD_PORT, 0xB6);
    outPortB(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outPortB(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);

    enable_speaker();
}

void stop_sound() {
    uint8_t tmp = inPortB(SPEAKER_CTRL_PORT);
    outPortB(SPEAKER_CTRL_PORT, tmp & ~3);
}

void play_song_impl(Song* song) {
    enable_speaker();

    for (uint32_t i = 0; i < song->length; i++) {
        Note note = song->notes[i];
        

        play_sound(note.frequency);
        sleep_interrupt(note.duration);
        stop_sound();
        

    }

    disable_speaker();
}
