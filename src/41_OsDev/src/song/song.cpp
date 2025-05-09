#include <song/song.h>
#include <kernel/interrupt/pit.h>
#include <kernel/memory/memory.h>

////////////////////////////////////////
// Song Playback Implementation
////////////////////////////////////////

// Play a song by looping through each note
void play_song_impl(Song* song) {
    for (uint32_t i = 0; i < song->length; i++) {
        Note* note = &song->notes[i];
        play_sound(note->frequency);           // Start tone
        sleep_interrupt(note->duration);       // Hold tone
        stop_sound();                          // Silence between notes
    }
}

////////////////////////////////////////
// Song Player Creation
////////////////////////////////////////

// Allocate and initialize a SongPlayer instance
SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song_impl;
    return player;
}
