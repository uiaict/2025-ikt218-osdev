#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include "song.h"

// Function to enable PC speaker
void enable_speaker();

// Function to disable PC speaker
void disable_speaker();

// Function to play a single sound frequency
void play_sound(uint32_t frequency);

// Function to stop sound
void stop_sound();

// Function to play an entire song
void play_song(Song *song);

// Function to create a song player object
SongPlayer* create_song_player();

#endif /* SONGPLAYER_H */