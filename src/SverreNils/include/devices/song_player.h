#ifndef SONG_PLAYER_H
#define SONG_PLAYER_H

#include <stdint.h>

void enable_speaker();
void disable_speaker();
void play_sound(uint32_t freq);
void stop_sound();

#endif
