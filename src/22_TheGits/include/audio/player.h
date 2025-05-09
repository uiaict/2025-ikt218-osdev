#ifndef PLAYER_H
#define PLAYER_H

#include "audio/song.h"

// Define a struct to represent a song player
typedef struct {
    void (*play_song)(Song* song); // Function pointer to a function that plays a song
} SongPlayer;

SongPlayer* create_song_player();
void play_song_impl(Song *song);
void play_music(Note* notes, uint32_t length);

#endif 
