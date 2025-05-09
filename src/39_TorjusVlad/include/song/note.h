#ifndef NOTE_H
#define NOTE_H

#include <libc/stdint.h>
#include <libc/stddef.h>

typedef struct {
    uint32_t frequency;   // Hz, 0 for pause
    uint32_t duration_ms; // milliseconds
} Note;

#endif