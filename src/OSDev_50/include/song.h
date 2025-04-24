#ifndef SONG_H
#define SONG_H

#include <stdint.h>
#include <stddef.h>

// One musical note: a frequency in Hz, and duration in ms
typedef struct {
    uint32_t frequency;
    uint32_t duration_ms;
} Note;

// A song is just a pointer to an array of notes plus its length
typedef struct {
    Note*     notes;
    size_t    note_count;
} Song;

// Example: if you have your first song in music_1.c as:
//   Note music_1[] = { {NOTE_C4, 500}, {NOTE_D4, 500}, … };
// then you’d extern it here:
extern Note music_1[];
extern size_t music_1_length;

// And group it into a Song:
static inline Song get_music_1_song(void) {
    return (Song){ .notes = music_1, .note_count = music_1_length };
}

#endif // SONG_H
