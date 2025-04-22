// filepath: /workspaces/2025-ikt218-osdev/src/25_postkasse/include/libc/song.h
#ifndef SONG_H
#define SONG_H

#include "libc/system.h"
#include "libc/frequencies.h"

//Structure of a note
typedef struct {
    uint32_t frequency;
    uint32_t duration;
} Note;

extern Note music_1[];
extern const size_t MUSIC_1_LENGTH;
#endif