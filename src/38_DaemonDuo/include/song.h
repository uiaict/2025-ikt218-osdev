#ifndef SONG_H
#define SONG_H

#include "libc/stdint.h"

// Structure to represent a musical note
struct note {
    uint32_t frequency;  // Frequency in Hz
    uint32_t duration;   // Duration in milliseconds
    uint32_t pause;      // Pause after the note in milliseconds
};

// Define the end marker for a song (zero frequency means end of song)
#define END_OF_SONG { 0, 0, 0 }

// Play a song defined as an array of notes
void play_song(const struct note song[]);

// External declaration for example_song
extern const struct note example_song[];

#endif // SONG_H
