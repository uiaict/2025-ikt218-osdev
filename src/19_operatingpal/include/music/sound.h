#pragma once

#include "libc/stdint.h"

void enable_speaker();
void disable_speaker();
void play_sound(uint32_t freq);
void stop_sound();
