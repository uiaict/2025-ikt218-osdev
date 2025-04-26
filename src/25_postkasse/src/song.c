// filepath: /workspaces/2025-ikt218-osdev/src/25_postkasse/src/song.c
#include "libc/song.h"

// Define the music_1 array
Note happy_birthday[] = {
    {E4, 250}, {G4, 250}, {A4, 500},
    {E4, 250}, {G4, 250}, {B4, 500},
    {E4, 250}, {G4, 250}, {C5, 500},
    {E4, 250}, {G4, 250}, {D5, 500},

    {F4, 250}, {A4, 250}, {C5, 500},
    {F4, 250}, {A4, 250}, {D5, 500},
    {F4, 250}, {A4, 250}, {E5, 500},
    {F4, 250}, {A4, 250}, {F5, 500}
};
const size_t HAPPY_BIRTHDAY_LENGTH = sizeof(happy_birthday) / sizeof(Note);

Note star_wars_theme[] = {
    {A4, 500}, {A4, 500}, {F4, 350}, {C5, 150},
    {A4, 500}, {F4, 350}, {C5, 150}, {A4, 1000},
    
    {E5, 500}, {E5, 500}, {E5, 500},
    {F5, 350}, {C5, 150}, {G4, 500}, {F4, 350}, {C5, 150}, {A4, 1000}
};
const size_t STAR_WARS_THEME_LENGTH = sizeof(star_wars_theme) / sizeof(Note);

Note fur_elise[] = {
    {E5, 250}, {Ds5, 250}, {E5, 250}, {Ds5, 250}, {E5, 250},
    {B4, 250}, {D5, 250}, {C5, 250}, {A4, 500},

    {C4, 250}, {E4, 250}, {A4, 250},
    {B4, 500},

    {E4, 250}, {G_SHARP4, 250}, {B4, 250},
    {C5, 500},

    {E4, 250}, {E5, 250}, {Ds5, 250}, {E5, 250}, {Ds5, 250}, {E5, 250},
    {B4, 250}, {D5, 250}, {C5, 250}, {A4, 500}
};
const size_t FUR_ELISE_LENGTH = sizeof(fur_elise) / sizeof(Note);




// Define the length of the song
