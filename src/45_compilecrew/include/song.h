#ifndef SONG_H
#define SONG_H

#include "frequencies.h"
#include "libc/system.h"

// Define a struct to represent a single musical note
typedef struct {
    uint32_t frequency; // Hz
    uint32_t duration;  // ms
} Note;

// Define a song
typedef struct {
    Note* notes;
    uint32_t length;
} Song;

// Define some static songs
static Note happy_birthday[] = {
    {G4, 400}, {G4, 400}, {A4, 800}, {G4, 800}, {C5, 800}, {B4, 1600},
    {G4, 400}, {G4, 400}, {A4, 800}, {G4, 800}, {D5, 800}, {C5, 1600},
    {G4, 400}, {G4, 400}, {G5, 800}, {E5, 800}, {C5, 800}, {B4, 800}, {A4, 1600},
    {F5, 400}, {F5, 400}, {E5, 800}, {C5, 800}, {D5, 800}, {C5, 1600}
};
static Song birthday = { happy_birthday, sizeof(happy_birthday) / sizeof(Note) };


static Note starwars_theme[] = {
    {A4, 500}, {A4, 500}, {F4, 350}, {C5, 150},
    {A4, 500}, {F4, 350}, {C5, 150}, {A4, 1000},

    {E5, 500}, {E5, 500}, {E5, 500},
    {F5, 350}, {C5, 150}, {G4, 500}, {F4, 350}, {C5, 150}, {A4, 1000}
};
static Song starwars = { starwars_theme, sizeof(starwars_theme) / sizeof(Note) };


static Note fur_elise[] = {
    {E5, 250}, {Ds5, 250}, {E5, 250}, {Ds5, 250}, {E5, 250},
    {B4, 250}, {D5, 250}, {C5, 250}, {A4, 500},

    {C4, 250}, {E4, 250}, {A4, 250},
    {B4, 500},

    {E4, 250}, {G_SHARP4, 250}, {B4, 250},
    {C5, 500},

    {E4, 250}, {E5, 250}, {Ds5, 250}, {E5, 250}, {Ds5, 250}, {E5, 250},
    {B4, 250}, {D5, 250}, {C5, 250}, {A4, 500}
};
static Song furelise = { fur_elise, sizeof(fur_elise) / sizeof(Note) };


#endif
