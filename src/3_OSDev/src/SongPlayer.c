#include <vga.h>
#include <utils.h>
#include <libc/stdio.h>
#include <memory/memory.h>
#include "pit.h"

#include <music_player/song.h>


void enable_speaker() {
    // [1] Read the current state from PC speaker control port
    uint8_t control = inb(PC_SPEAKER_PORT);
    // [3] Set both bits 0 and 1 to enable the speaker
    control |= 0x03;
    outb(PC_SPEAKER_PORT, control);
}

void disable_speaker() {
    // [1] Read the current state from PC speaker control port
    uint8_t control = inb(PC_SPEAKER_PORT);
    // [2] Clear both bits 0 and 1 to disable the speaker
    control &= ~0x03;
    outb(PC_SPEAKER_PORT, control);
}

void play_sound(uint32_t frequency) {
    // [1] Check if frequency is 0
    if (frequency == 0) {
        disable_speaker();
        return;
    }
    // [2] Calculate the divisor
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;
    // [3] Configure the PIT to desired frequency
    outb(PIT_CMD_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF)); // Low byte
    outb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // High byte
    // [4] Enable the speaker
    enable_speaker();
}

void stop_sound() {
    // [1] Read the current state from PC speaker control port
    uint8_t control = inb(PC_SPEAKER_PORT);
    // [2] Disable the speaker
    control &= ~0x03; 
}

void play_song_impl(Song *song) {
    // [1] Enable the speaker
    enable_speaker();
    // [2] Loop through each note in the song's note array
    for (size_t i = 0; i < song->note_count; i++) {
        // [a] For each note, display its details such as frequency and duration
        Note note = song->notes[i];
        // printf(0x0F, "Playing note: Frequency = %d, Duration = %d\n", note.frequency, note.duration);
        // [b] Call play_sound with the note's frequency
        play_sound(note.frequency);
        // [c] Delay execution for the duration of the note
        sleep_busy(note.duration);
        // [d] Call stop_sound to end the note
        stop_sound();
    }
    // [3] Disable the speaker after the song is finished
    disable_speaker();
}

void play_song(Song *song) {
    play_song_impl(song);
}

SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song_impl;
    return player;
}