#pragma once

#include "music_player/song_player.h"

extern Song fur_elise_song;
extern Song twinkle_twinkle_song;
extern Song happy_birthday_song;

extern Song* songList;
extern size_t numOfSongs;

void init_song_library();
void destroy_song_library();
void list_songs();