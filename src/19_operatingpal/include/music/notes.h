#pragma once

#include "libc/stdint.h"
#include "libc/stddef.h"

// Represents a single note (frequency in Hz, duration in ms)
typedef struct {
    uint32_t frequency;
    uint32_t duration;
} Note;

// Represents a song as a list of notes
typedef struct {
    Note* notes;
    size_t note_count;
} Song;
