#ifndef SONG_H
#define SONG_H

#include <libc/stdint.h>

typedef struct
{
    uint32_t frequency;   // In Hertz
    uint32_t duration_ms; // In milliseconds
} Note;

typedef struct
{
    Note *notes;
    uint32_t length;
} Song;

#endif
