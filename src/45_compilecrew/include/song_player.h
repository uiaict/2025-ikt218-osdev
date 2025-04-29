#ifndef SONG_PLAYER_H
#define SONG_PLAYER_H

#include "song.h"

typedef struct {
    void (*play_song)(Song* song);
} SongPlayer;

void play_song_impl(Song* song);
void play_song(Song* song);
SongPlayer* create_song_player();

#endif
