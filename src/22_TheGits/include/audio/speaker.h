#ifndef SPEAKER_H
#define SPEAKER_H

#include "libc/stdint.h"

void enable_speaker();
void disable_speaker();
void play_sound(uint32_t frequency);
void stop_sound();
void play_success_melody();
void play_error_melody();


#endif 
