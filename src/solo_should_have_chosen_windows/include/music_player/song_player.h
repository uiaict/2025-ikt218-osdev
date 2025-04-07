#pragma once

#include "libc/stdint.h"

typedef struct {
    uint32_t frequency;
    uint32_t duration;
} Note;

typedef struct {
    char* title;
    char* artist;
    Note *notes;
    size_t note_count;
} Song;

typedef struct {
    void (*play_song)(Song *song);
} SongPlayer;

SongPlayer *create_song_player();

void playAllSongs(Song* songs, size_t song_count);
void playSong(Song *song);

void destroy_song_player(SongPlayer *player);