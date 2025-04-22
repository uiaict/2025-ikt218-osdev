#ifndef SPEAKER_H
#define SPEAKER_H

#include "libc/stdint.h"
#include "libc/song.h"

// Enables the PC speaker by setting the appropriate bits on port 0x61
void enable_speaker();

// Disables the PC speaker by clearing the appropriate bits on port 0x61
void disable_speaker();

void play_sound(uint32_t frequency);

void stop_sound();
void play_song(Note* notes, size_t length);
#endif // SPEAKER_H