#include <stddef.h>        // For size_t
#include "song/note.h"     // Note-struktur

// Imperial March - for PC speaker
Note music_1[] = {
    {440, 500},   // A4
    {440, 500},   // A4
    {440, 500},   // A4
    {349, 350},   // F4
    {523, 150},   // C5
    {440, 500},   // A4
    {349, 350},   // F4
    {523, 150},   // C5
    {440, 1000},  // A4 (lang)

    {659, 500},   // E5
    {659, 500},   // E5
    {659, 500},   // E5
    {698, 350},   // F5
    {523, 150},   // C5
    {415, 500},   // G#4
    {349, 350},   // F4
    {523, 150},   // C5
    {440, 1000},  // A4
};

const size_t music_1_len = sizeof(music_1) / sizeof(Note);
