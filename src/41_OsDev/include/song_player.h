//song_player.h

#ifndef SONG_PLAYER_H
#define SONG_PLAYER_H

#include "song.h"

// Play the given song
void play_song(Song* song);

// Play song implementation used by SongPlayer
void play_song_impl(Song* song);

#endif
