#ifndef MUSIC_H
#define MUSIC_H

#include "libc/stdint.h"

// Function declarations for the music player
void init_music(void);
void play_wav(const char *filename);
void stop_music(void);

#endif // MUSIC_H
