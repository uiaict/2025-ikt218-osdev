#ifndef SONG_H
#define SONG_H

#include "frequencies.h"
#include "libc/stdint.h"
// #include "libc/system.h"

// Define a struct to represent a single musical note
struct note{
    uint32_t frequency; // The frequency of the note in Hz
    uint32_t duration;  // The duration of the note in ms
};

// Define a struct to represent a song
struct song{
    struct note* notes;  // Pointer to an array of Note structs representing the song
    uint32_t length;    // The number of notes in the song
};

// Define a struct to represent a song player
struct song_player{
    void (*play_song)(struct song*); // Function pointer to a function that plays a song
};

// Function prototype for creating a new SongPlayer instance
// Returns a pointer to a newly created SongPlayer object
struct song_player* create_song_player();
void play_song(struct song*); // Disables speaker to prevent white noise after

static struct note SMB_1_1[] = {
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

static struct note imperial_march[] = {
    // Opening phrase
    {A4, 500}, {A4, 500}, {A4, 500}, 
    {F4, 375}, {C5, 125}, 
    {A4, 500}, {F4, 375}, {C5, 125}, {A4, 1000}, 
    {E5, 500}, {E5, 500}, {E5, 500}, 
    {F5, 375}, {C5, 125},
    
    // Next phrase
    {G4, 500}, {F4, 375}, {C5, 125}, {A4, 1000}, 
    {A5, 500}, {A4, 375}, {A4, 125}, 
    {A5, 500}, {G5, 375}, {F5, 125}, {E5, 125}, {D5, 125}, 
    {C5, 250}, {B4, 250}, {A4, 500},

    // End note
    {R, 500}
};


static struct note battlefield_1942_theme[] = {
    // Attempt at the opening part of the Battlefield 1942 theme
    {E4, 500}, {G4, 500}, {B4, 300}, {E5, 200}, 
    {D5, 200}, {B4, 300}, {G4, 500}, {B4, 300}, 
    {E5, 200}, {D5, 200}, {B4, 300}, {G4, 500}, 
    {B4, 300}, {E5, 200}, {G5, 200}, {E5, 300}, 

    // Continue with the next part of the melody
    {D5, 200}, {B4, 300}, {G4, 500}, {E4, 500}, 
    {G4, 500}, {B4, 300}, {E5, 200}, {D5, 200}, 
    {B4, 300}, {G4, 500}, {B4, 300}, {E5, 200}, 
    {D5, 200}, {B4, 300}, {G4, 500}, {B4, 300}, 
    {E5, 200}, {G5, 200}, {E5, 300}, {D5, 200}, 
    {B4, 300}, {G4, 500}, 

    // Repeat or modify as needed
    // ...

    // End note
    {R, 500}
};


static struct note music_2[] = {
    {A4, 200}, {E5, 200}, {A5, 200}, {R, 100}, {A5, 200}, {A5, 200}, {Gs5, 200}, {A5, 200},
    {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200},
    {A4, 200}, {E5, 200}, {A5, 200}, {R, 100}, {A5, 200}, {A5, 200}, {Gs5, 200}, {A5, 200},
    {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200},
    {A4, 200}, {E5, 200}, {A5, 200}, {R, 100}, {A5, 200}, {A5, 200}, {Gs5, 200}, {A5, 200},
    {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}, {R, 100}, {E5, 200}
};

static struct note ode_to_joy[] = {
    {E4, 200}, {E4, 200}, {F4, 200}, {G4, 200}, {G4, 200}, {F4, 200}, {E4, 200}, {D4, 200},
    {C4, 200}, {C4, 200}, {D4, 200}, {E4, 200}, {E4, 400}, {R, 200},
    {D4, 200}, {D4, 200}, {E4, 200}, {F4, 200}, {F4, 200}, {E4, 200}, {D4, 200}, {C4, 200},
    {A4, 200}, {A4, 200}, {A4, 200}, {G4, 400}
};

static struct note brother_john[] = { // Fader Jakob er "Brother John" på engelsk???
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500},
    {C4, 500}, {D4, 500}, {E4, 500}, {C4, 500},
    {E4, 500}, {F4, 500}, {G4, 1000},
    {E4, 500}, {F4, 500}, {G4, 1000},
    {G4, 250}, {A4, 250}, {G4, 250}, {F4, 250}, {E4, 500}, {C4, 500},
    {G4, 250}, {A4, 250}, {G4, 250}, {F4, 250}, {E4, 500}, {C4, 500},
    {C4, 500}, {G3, 500}, {C4, 1000},
    {C4, 500}, {G3, 500}, {C4, 1000}
};

static struct note music_5[] = {
    {E4, 375}, {C4, 375}, {D4, 375}, {A3, 375}, {B3, 375}, {D4, 375}, {C4, 375}, {A3, 375},
    {E4, 375}, {C4, 375}, {D4, 375}, {A3, 375}, {B3, 375}, {D4, 375}, {C4, 375}, {A3, 375},
};

static struct note music_6[] = {
    {F4, 250}, {F4, 250}, {F4, 250}, {C5, 250}, {A_SHARP4, 250}, {G_SHARP4, 250}, {F4, 500},
    {F4, 250}, {F4, 250}, {F4, 250}, {C5, 250}, {A_SHARP4, 250}, {G_SHARP4, 250}, {F4, 500},
    {A_SHARP4, 250}, {A_SHARP4, 250}, {A_SHARP4, 250}, {F5, 250}, {D5, 250}, {C5, 250}, {A_SHARP4, 500},
    {A_SHARP4, 250}, {A_SHARP4, 250}, {A_SHARP4, 250}, {F5, 250}, {D5, 250}, {C5, 250}, {A_SHARP4, 500},
};

static struct note megalovania[] = {
    {293, 140}, {293, 122}, {587, 248}, {440, 368}, {415, 256},
    {392, 242}, {349, 251}, {293, 121}, {349, 121}, {392, 128},
    {261, 140}, {261, 122}, {587, 248}, {440, 368}, {415, 256},
    {392, 242}, {349, 251}, {293, 121}, {349, 121}, {392, 128},
    {246, 140}, {246, 122}, {587, 248}, {440, 368}, {415, 256},
    {392, 242}, {349, 251}, {293, 121}, {349, 121}, {392, 128},
    {233, 140}, {233, 122}, {587, 248}, {440, 368}, {415, 256},
    {392, 242}, {349, 251}, {293, 121}, {349, 121}, {392, 128},

    {293, 140}, {293, 122}, {587, 248}, {440, 368}, {415, 256},
    {392, 242}, {349, 251}, {293, 121}, {349, 121}, {392, 128},
    {261, 140}, {261, 122}, {587, 248}, {440, 368}, {415, 256},
    {392, 242}, {349, 251}, {293, 121}, {349, 121}, {392, 128},
    {246, 140}, {246, 122}, {587, 248}, {440, 368}, {415, 256},
    {392, 242}, {349, 251}, {293, 121}, {349, 121}, {392, 128},
    {233, 140}, {233, 122}, {587, 248}, {440, 368}, {415, 256},
    {392, 242}, {349, 251}, {293, 121}, {349, 121}, {392, 128}
};

#endif // SONG_H
