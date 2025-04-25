#ifndef SOUND_H
#define SOUND_H

#include <libc/stdint.h>

void enable_speaker();
void disable_speaker();
void play_sound(uint32_t frequency);
void stop_sound();
void speaker_beep(uint32_t frequency, uint32_t duration_ms);

#endif