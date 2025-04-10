#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "libc/stddef.h"
#include "libc/stdint.h"

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
void enable_speaker(void);
void disable_speaker(void);
void play_sound(uint32_t frequency);
void stop_sound(void);
void play_song_impl(Song* song);
SongPlayer* create_song_player(void);

#endif // MUSICPLAYER_H