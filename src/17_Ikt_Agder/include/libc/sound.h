#ifndef SOUND_H
#define SOUND_H

#include "libc/stdint.h"

// Function declarations for the sound driver
void init_sound(void);
void play_sound(uint8_t *data, size_t length);
void stop_sound(void);

#endif // SOUND_H
