#ifndef PLAYER_H
#define PLAYER_H

#include "audio/song.h" // Include the header file for the Song struct

// Define a struct to represent a song player
typedef struct {
    void (*play_song)(Song* song); // Function pointer to a function that plays a song
} SongPlayer;

// Function prototype for creating a new SongPlayer instance
// Returns a pointer to a newly created SongPlayer object
SongPlayer* create_song_player();
void play_song_impl(Song *song) ;

#endif 
