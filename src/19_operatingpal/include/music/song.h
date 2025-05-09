#pragma once

#include "music/notes.h"

// Starts playing a song
void play_song(Song* song);

// Advances to the next note (called by timer)
void update_song_tick();

// Stops current song
void stop_song();

// Clears the screen
void clear_screen();
