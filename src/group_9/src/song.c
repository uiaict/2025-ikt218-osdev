#include "song.h"
#include "speaker.h"
#include "pit.h"
#include "terminal.h"
#include "memory.h"

// Implementation to play a song note-by-note
void play_song_impl(Song* song) {
    enable_speaker();

    for (uint32_t i = 0; i < song->length; i++) {
        Note note = song->notes[i];
        terminal_printf("Note %d: Freq=%d Hz, Sleep=%d ms\n", i, note.frequency, note.duration);
        play_sound(note.frequency);
        sleep_interrupt(note.duration);
        stop_sound();
    }

    disable_speaker();
}

// Create a SongPlayer and set play_song pointer
SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song_impl;
    return player;
}
