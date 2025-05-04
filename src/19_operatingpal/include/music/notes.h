#pragma once

#include "libc/stdint.h"
#include "libc/stddef.h"

typedef struct {
    uint32_t frequency;
    uint32_t duration;
} Note;

typedef struct {
    Note* notes;
    size_t note_count;
} Song;
