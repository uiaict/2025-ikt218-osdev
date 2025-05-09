#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "common.h"

// Initialize the music player
void init_music_player();

// Play built-in song by index
void music_play_song(u8int song_index);

// Stop currently playing song
void music_stop();

// Number of built-in songs available
u8int music_get_song_count();

// Get the name of a song by index
const char* music_get_song_name(u8int song_index);

// Enter piano mode where keyboard keys play notes
void music_enter_piano_mode();

// Exit piano mode
void music_exit_piano_mode();

// Check if we're in piano mode
u8int music_is_piano_mode();

// Play a note in piano mode
void music_play_piano_note(char key);

#endif