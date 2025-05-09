#pragma once

#include "libc/stdint.h"

// Turns on PC speaker
void enable_speaker();

// Turns off PC speaker
void disable_speaker();

// Plays sound at given frequency
void play_sound(uint32_t freq);

// Stops current sound
void stop_sound();
