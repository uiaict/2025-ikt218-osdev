#pragma once
#include "note.h"
#include <stddef.h>

typedef struct {
    Note* notes;
    size_t note_count;
} Song;
