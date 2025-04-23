#ifndef SONG_H
#define SONG_H

#include "libc/stddef.h"  // for size_t
#include "libc/stdint.h"  // for uint32_t

#ifdef __cplusplus
extern "C" {
#endif

// Represents a single musical note (frequency in Hz, duration in ms)
typedef struct {
    uint32_t frequency;
    uint32_t duration;
} Note;

// Represents a song (an array of notes + how many)
typedef struct {
    Note* notes;
    size_t note_count;
} Song;

// A "song player" object with a function pointer to play a song
typedef struct SongPlayer {
    void (*play_song)(Song* song);
} SongPlayer;

// Creates a new SongPlayer that knows how to play songs
SongPlayer* create_song_player(void);

// Function to set the PC speaker frequency
void set_speaker_frequency(uint32_t frequency);

#ifdef __cplusplus
}
#endif

#endif // SONG_H