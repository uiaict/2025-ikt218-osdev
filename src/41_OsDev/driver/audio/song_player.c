#include <libc/stdint.h>
#include <libc/stdio.h>
#include <kernel/interrupt/pit.h>
#include <kernel/common.h>
#include <song/song.h>
#include <kernel/memory/memory.h>
#include <driver/include/terminal.h>

////////////////////////////////////////
// Speaker Control
////////////////////////////////////////

// Disable PC speaker output
void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC);
}

// Enable PC speaker output
void enable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 0x03);
}

////////////////////////////////////////
// Sound Generation via PIT Channel 2
////////////////////////////////////////

// Generate a tone with the given frequency (Hz)
void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        disable_speaker();
        return;
    }

    uint32_t divisor = PIT_FREQUENCY / frequency;

    if (divisor == 0) divisor = 1;
    if (divisor > 0xFFFF) divisor = 0xFFFF;

    outb(PIT_COMMAND, 0xB6);
    outb(PIT_CHANNEL2, divisor & 0xFF);
    outb(PIT_CHANNEL2, (divisor >> 8) & 0xFF);

    enable_speaker();
}

// Stop any sound from the PC speaker
void stop_sound() {
    disable_speaker();
}

////////////////////////////////////////
// Song Playback Engine
////////////////////////////////////////

// Play the given song using the PC speaker
void play_song_impl(Song *song) {
    printf("Playing song...\n");

    stop_sound();
    sleep_interrupt(100);

    for (size_t i = 0; i < song->length; i++) {
        if (song->notes[i].frequency == 0 && song->notes[i].duration == 0) {
            break;
        }

        play_sound(song->notes[i].frequency);
        sleep_interrupt(song->notes[i].duration);
        stop_sound();
        sleep_interrupt(20);
    }

    stop_sound();
    printf("Song finished.\n");
}

////////////////////////////////////////
// Song Player Factory
////////////////////////////////////////

// Allocate and return a new SongPlayer instance
SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song_impl;
    return player;
}
