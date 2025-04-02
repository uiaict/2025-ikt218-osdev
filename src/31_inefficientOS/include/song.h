#ifndef SONG_H
#define SONG_H

#include "libc/stdint.h"
#include "notes.h"

typedef struct {
    uint32_t frequency; // Frequency of the note in Hz
    uint32_t duration;  // How long the note should last in milliseconds
} Note;

// Define a struct to represent a song
typedef struct {
    Note* notes;        // Pointer to an array of Note structs representing the song
    uint32_t length;    // The number of notes in the song
} Song;

// Define a struct to represent a song player
typedef struct {
    void (*play_song)(Song song); // Function pointer to a function that plays a song
} SongPlayer;

// Function prototype for creating a new SongPlayer instance
// Returns a pointer to a newly created SongPlayer object
SongPlayer* create_song_player();
void play_song_impl(Song *song);

// Pink Floyd - Another Brick in the Wall (Part 2) - with correct rhythm
static Note another_brick_notes[] = {
    // Intro - iconic bass line in D minor
    {D3, 600}, {A3, 600}, {D3, 600}, {A3, 600},
    {D3, 600}, {A3, 600}, {D3, 600}, {A3, 600},
    
    // "We don't need no education" - main vocal line
    {D4, 500}, {D4, 500}, {C4, 400}, {D4, 600},
    {F4, 600}, {D4, 800},
    
    {C4, 500}, {As3, 500}, {C4, 400}, {D4, 600},
    {C4, 800}, {R, 200},
    
    // "We don't need no thought control"
    {D4, 500}, {D4, 500}, {C4, 400}, {D4, 600},
    {F4, 600}, {D4, 800},
    
    {C4, 500}, {As3, 500}, {A3, 400}, {G3, 400},
    {F3, 400}, {G3, 800}, {R, 200},
    
    // "No dark sarcasm in the classroom"
    {D4, 400}, {D4, 500}, {C4, 400}, {D4, 600},
    {F4, 600}, {D4, 800},
    
    {C4, 500}, {As3, 500}, {C4, 400}, {D4, 600},
    {C4, 800}, {R, 200},
    
    // "Teacher, leave them kids alone"
    {D4, 500}, {F4, 500}, {G4, 700},
    {F4, 500}, {E4, 500}, {D4, 1000},
    {R, 400},
    
    // "Hey, teacher, leave those kids alone!"
    {D4, 400}, {F4, 400}, {G4, 800},
    {F4, 400}, {E4, 400}, {F4, 400}, {E4, 400}, {D4, 800},
    {R, 400},
    
    // Guitar solo part after chorus
    {D5, 600}, {C5, 600}, {G4, 800},
    {D5, 600}, {C5, 600}, {G4, 800},
    {D5, 600}, {C5, 600}, {G4, 1000},
    {R, 400}
};


// Define a song structure for Another Brick in the Wall
static Song another_brick = {
    another_brick_notes,
    sizeof(another_brick_notes) / sizeof(Note)
};

#endif // SONG_H