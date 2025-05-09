#ifndef MUSIC_H
#define MUSIC_H

#include "music/notes.h"
#include "libc/stdint.h"

// Same as the original code

typedef struct {
    uint32_t frequency;
    uint32_t duration;
} Note;

typedef struct {
    Note *notes;
    uint32_t length;
} Song;

typedef struct {
    void (*play_song)(Song* song);
} SongPlayer;

SongPlayer* create_song_player();
void play_song(Song* song);

// Taken from assigment zip
static Note starwars_theme[] = {
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

extern Song starwars_song;
#endif
