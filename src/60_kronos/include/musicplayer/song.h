#ifndef SONG_H
#define SONG_H

#include "frequencies.h"
#include "libc/stdint.h"

// Define a struct to represent a single musical note
typedef struct {
    uint32_t frequency; // The frequency of the note in Hz (e.g., A4 = 440 Hz)
    uint32_t duration;  // The duration of the note in milliseconds
} Note;

// Define a struct to represent a song
typedef struct {
    Note* notes;        // Pointer to an array of Note structs representing the song
    uint32_t length;    // The number of notes in the song
} Song;

// Define a struct to represent a song player
typedef struct {
    void (*play_song)(Song* song); // Function pointer to a function that plays a song
} SongPlayer;

// Function prototype for creating a new SongPlayer instance
// Returns a pointer to a newly created SongPlayer object
#ifdef __cplusplus
extern "C" {
SongPlayer* create_song_player();
void play_song_impl(Song *song);
}
#endif

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

static Note song_of_time[] = {
    {G4, 400}, {C4, 800}, {E4, 400},
    {G4, 400}, {C4, 800}, {E4, 400},
    {G4, 200}, {B4, 200}, {A4, 400}, {F4, 400}, {E4, 200}, {F4, 200},
    {G4, 400}, {C4, 400}, {B3, 200}, {D4, 200}, {C4, 2000},   
};



static Note battlefield_1942_theme[] = {
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

#endif
