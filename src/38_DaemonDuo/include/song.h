#pragma once
#include "libc/stdint.h"

// Changed from uint8_t to uint16_t to handle larger frequency values
extern const uint16_t example_song[];

void play_song(const uint16_t* song_data);
