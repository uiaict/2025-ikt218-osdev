#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "libc/stddef.h"
#include "libc/stdint.h"
#include "pit.h"  // Include pit.h for speaker functions

// A single note, defined by its frequency (Hz) and duration (ms)
typedef struct {
    uint32_t freq;
    uint32_t duration;
} Note;

// A song: an array of notes and a count
typedef struct {
    Note* notes;
    size_t note_count;
} Song;

// SongPlayer structure holding a pointer to the song-playing function
typedef struct {
    void (*play_song)(Song* song);
} SongPlayer;

// Function prototypes
void play_song_impl(Song* song);
SongPlayer* create_song_player(void);

// Note: The following functions are already defined in pit.c
// and should NOT be redefined in musicplayer.c:
// - init_pc_speaker()
// - beep()
// - beep_blocking()

#endif // MUSICPLAYER_H