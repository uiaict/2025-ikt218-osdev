// filepath: /workspaces/2025-ikt218-osdev/src/25_postkasse/src/song.c
#include "libc/song.h"

// Define the music_1 array
Note music_1[] = {
    {C4, 250}, {C4, 125}, {D4, 500}, {C4, 500}, {F4, 500}, {E4, 1000},
    
    {C4, 250}, {C4, 125}, {D4, 500}, {C4, 500}, {G4, 500}, {F4, 1000},
    
    {C4, 250}, {C4, 125}, {C5, 500}, {A4, 500}, {F4, 500}, {E4, 500}, {D4, 1000},
    
    {A_SHARP4, 250}, {A_SHARP4, 125}, {A4, 500}, {F4, 500}, {G4, 500}, {F4, 1000}
};


// Define the length of the song
const size_t MUSIC_1_LENGTH = sizeof(music_1) / sizeof(Note);