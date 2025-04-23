#pragma once
#include "libc/stdint.h"
#include "song/song_player.h"

extern const uint8_t example_song[];

void play_song(const uint8_t* song_data);
