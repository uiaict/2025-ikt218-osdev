#ifndef SONG_PLAYER_H
#define SONG_PLAYER_H

#include "song/note.h"
#include "libc/stdbool.h"
#include <libc/stddef.h>

typedef struct {
    Note* notes;
    size_t note_count;
} Song;

typedef struct {
    bool (*play_song)(Song* song);
} SongPlayer;

SongPlayer* create_song_player();
bool play_song_impl(Song* song);

#endif
