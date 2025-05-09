#ifndef SONG_H
#define SONG_H

#include "frequencies.h"
#include "libc/stdint.h"

// Represents a single note with frequency and duration
typedef struct {
    uint32_t frequency; // Hz (0 for silence)
    uint32_t duration;  // milliseconds
} Note;

// Represents a full song composed of multiple notes
typedef struct {
    Note* notes;
    uint32_t length;
} Song;

// Song player structure
typedef struct {
    void (*play_song)(Song* song);
} SongPlayer;

// Function prototypes
SongPlayer* create_song_player();
void play_song_impl(Song* song);

// === SONG DATA ===

static Note music_1[] = {
    {E5, 250}, {R, 125}, {E5, 125}, {R, 125}, {E5, 125}, {R, 125},
    {C5, 125}, {E5, 125}, {G5, 125}, {R, 125}, {G4, 125}, {R, 250},

    {C5, 125}, {R, 250}, {G4, 125}, {R, 125}, {E4, 125}, {R, 125},
    {A4, 125}, {B4, 125}, {R, 125}, {A_SHARP4, 125}, {A4, 125}, {R, 125},
    {G4, 125}, {E5, 125}, {G5, 125}, {A5, 125}, {F5, 125}, {G5, 125},
    {R, 125}, {E5, 125}, {C5, 125}, {D5, 125}, {B4, 125}, {R, 125},

    {C5, 125}, {R, 250}, {G4, 125}, {R, 125}, {E4, 125}, {R, 125},
    {A4, 125}, {B4, 125}, {R, 125}, {A_SHARP4, 125}, {A4, 125}, {R, 125},
    {G4, 125}, {E5, 125}, {G5, 125}, {A5, 125}, {F5, 125}, {G5, 125},
    {R, 125}, {E5, 125}, {C5, 125}, {D5, 125}, {B4, 125}, {R, 125},
};

static Note music_2[] = {
    {A4, 200}, {E5, 200}, {A5, 200}, {R, 100}, {A5, 200}, {A5, 200}, {G_SHARP4, 200}, {A5, 200},
    {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200},
    {A4, 200}, {E5, 200}, {A5, 200}, {R, 100}, {A5, 200}, {A5, 200}, {G_SHARP4, 200}, {A5, 200},
    {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200},
};

static Note music_3[] = {
    {E4, 200}, {E4, 200}, {F4, 200}, {G4, 200}, {G4, 200}, {F4, 200}, {E4, 200}, {D4, 200},
    {C4, 200}, {C4, 200}, {D4, 200}, {E4, 200}, {E4, 400}, {R, 200},
    {D4, 200}, {D4, 200}, {E4, 200}, {F4, 200}, {F4, 200}, {E4, 200}, {D4, 200}, {C4, 200},
    {A4, 200}, {A4, 200}, {A4, 200}, {G4, 400}
};

static Note music_4[] = {
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500},
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500},
    {E4, 500}, {F4, 500}, {G4, 1000},
    {E4, 500}, {F4, 500}, {G4, 1000},
    {G4, 250}, {A4, 250}, {G4, 250}, {F4, 250}, {E4, 500}, {C4, 500},
    {G4, 250}, {A4, 250}, {G4, 250}, {F4, 250}, {E4, 500}, {C4, 500},
};

#endif // SONG_H
