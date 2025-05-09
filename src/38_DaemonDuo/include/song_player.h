#ifndef SONG_PLAYER_H
#define SONG_PLAYER_H

#include <libc/stdint.h>
#include "song.h"

// Function to play a sound at the specified frequency
void play_sound(uint32_t frequency);

// Function to stop the sound
void stop_sound();

// Function to introduce a delay
void delay(uint32_t duration);

// Functions to control PC speaker
void enable_speaker();
void disable_speaker();

#endif // SONG_PLAYER_H
