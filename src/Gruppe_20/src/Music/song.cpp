#include "Music/song.h"
#include "pit.h"
#include "libc/stdio.h"
#include "io.h"



void enable_speaker() {
    uint8_t state = inb(PC_SPEAKER_PORT);
    if (!(state & 3)) {
        outb(PC_SPEAKER_PORT, state | 3);
    }
}

void disable_speaker() {
    uint8_t state = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, state & 0xFC);
}

void play_sound(uint32_t freq) {
    if (!freq) return;
    
    uint16_t div = PIT_BASE_FREQUENCY / freq;
    outb(PIT_CMD_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, div & 0xFF);
    outb(PIT_CHANNEL2_PORT, (div >> 8) & 0xFF);
}

void play_song_impl(Song* song) {
    enable_speaker();
    for (uint32_t i = 0; i < song->length; i++) {
        play_sound(song->notes[i].frequency);
        sleep_interrupt(song->notes[i].duration);
    }
    disable_speaker();
}

SongPlayer* create_song_player() {
    auto* player = new SongPlayer();
    player->play_song = play_song_impl;
    return player;
}