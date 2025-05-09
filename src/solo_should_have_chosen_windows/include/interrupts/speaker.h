#pragma once

#include "libc/stdint.h"

void speaker_play_frequency(uint32_t frequency);
void speaker_stop();
void speaker_beep(uint32_t frequency, uint32_t duration_ms);