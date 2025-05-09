#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H
#include "song.h"
#include "../PIT/PIT.h"
#include "libc/stdio.h"
#include "../util/util.h"
#include "libc/stdbool.h"
#include "../vga/vga.h"

void enable_speaker();
void disable_speaker();
void play_sound(uint32_t frequency);
void stop_sound();
void play_song_impl(Song *song);
void play_song(Song *song);
#endif
