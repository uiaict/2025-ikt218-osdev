#ifndef SONG_H
#define SONG_H

#include "frequencies.h"
#include <libc/stdint.h>

////////////////////////////////////////
// Note and Song Structures
////////////////////////////////////////

// Single musical note (frequency in Hz, duration in ms)
typedef struct {
    uint32_t frequency;
    uint32_t duration;
} Note;

// Song composed of multiple notes
typedef struct {
    Note* notes;
    uint32_t length;
    uint32_t id;  // Optional identifier (for debugging)
} Song;

// Song player interface
typedef struct {
    void (*play_song)(Song* song);
} SongPlayer;

////////////////////////////////////////
// Song Player API
////////////////////////////////////////

// Create a new SongPlayer instance
SongPlayer* create_song_player(void);

// Internal implementation of song playback
void play_song_impl(Song* song);

// Low-level sound control
void play_sound(uint32_t frequency);
void stop_sound(void);

////////////////////////////////////////
// Predefined Songs (from songs.c)
////////////////////////////////////////

extern Note music_1[];
extern Note starwars_theme[];
extern Note battlefield_1942_theme[];
extern Note music_2[];
extern Note music_3[];
extern Note music_4[];
extern Note music_5[];
extern Note music_6[];

#endif // SONG_H
