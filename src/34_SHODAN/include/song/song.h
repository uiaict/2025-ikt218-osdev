#ifndef SONG_H
#define SONG_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t frequency; // Hz
    uint32_t duration;  // ms
} Note;

typedef struct {
    Note* notes;
    size_t note_count;
} Song;

#define R 0  // Rest

#endif
