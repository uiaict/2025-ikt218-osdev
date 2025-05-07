#ifndef SONG_PLAYER_H
#define SONG_PLAYER_H

#include "song.h"
#include "libc/stdint.h"

void enable_speaker();
void disable_speaker();
void stop_sound();
void play_sound(uint32_t frequency);
void play_song_impl(Song *song);
void play_song(Song *song);
SongPlayer *create_song_player();

#endif