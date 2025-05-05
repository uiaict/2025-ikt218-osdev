#ifndef SONG_H
#define SONG_H

#include "frequencies.h"
#include "libc/system.h"

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
SongPlayer* create_song_player();
void play_song_impl(Song *song) ;

// Castlevania - Vampire Killer theme (improved timing and notes)
static Note castlevania_theme[] = {
    // Intro phrase - establish the key rhythm
    {C5, 130}, {D5, 130}, {F5, 130}, {G5, 130},
    {A5, 260}, {G5, 130}, {F5, 130}, {E5, 130}, {F5, 130},
    {D5, 260}, {F5, 130}, {G5, 130}, {A5, 130}, {C6, 260},

    // Second phrase
    {A5, 130}, {G5, 130}, {F5, 130}, {G5, 130},
    {E5, 260}, {F5, 130}, {E5, 130}, {D5, 130}, {C5, 390},
    
    // Brief rest between sections
    {R, 130},
    
    // Repeat of first phrase with slight variation
    {C5, 130}, {D5, 130}, {F5, 130}, {G5, 130},
    {A5, 260}, {G5, 130}, {F5, 130}, {E5, 130}, {F5, 130},
    {D5, 260}, {F5, 130}, {G5, 130}, {A5, 130}, {C6, 260},
    
    // Final phrase with longer ending note
    {A5, 130}, {G5, 130}, {F5, 130}, {G5, 130},
    {E5, 260}, {F5, 130}, {E5, 130}, {D5, 130}, {C5, 390},
    
    // End with a flourish
    {F5, 130}, {G5, 130}, {A5, 260}, {G5, 130}, {F5, 260}, {C5, 390},
    {R, 260}
};

// Super Mario Bros theme - properly timed for PC speaker
static Note mario_theme[] = {
    // Opening phrase
    {E5, 120}, {R, 40}, {E5, 120}, {R, 80}, {E5, 120}, {R, 80},
    {C5, 120}, {E5, 120}, {G5, 240}, {R, 240}, {G4, 240}, {R, 240},
    
    // Second phrase
    {C5, 240}, {R, 120}, {G4, 240}, {R, 120}, {E4, 240}, {R, 120},
    {A4, 120}, {B4, 120}, {R, 60}, {As4, 120}, {A4, 240}, {R, 60},
    
    // Third phrase - with triplets
    {G4, 120}, {E5, 120}, {G5, 120}, {A5, 240}, {F5, 120}, {G5, 120},
    {R, 60}, {E5, 120}, {C5, 120}, {D5, 120}, {B4, 240}, {R, 120},
    
    // Repeat of second phrase
    {C5, 240}, {R, 120}, {G4, 240}, {R, 120}, {E4, 240}, {R, 120},
    {A4, 120}, {B4, 120}, {R, 60}, {As4, 120}, {A4, 240}, {R, 60},
    
    // Repeat of third phrase
    {G4, 120}, {E5, 120}, {G5, 120}, {A5, 240}, {F5, 120}, {G5, 120},
    {R, 60}, {E5, 120}, {C5, 120}, {D5, 120}, {B4, 240}, {R, 240}
};

// Battlefield 1942 theme - improved timing
static Note battlefield_1942_theme[] = {
    // Main theme - heroic brass melody
    {E4, 400}, {G4, 400}, {B4, 250}, {E5, 200},
    {D5, 200}, {B4, 250}, {G4, 400}, {B4, 250},
    {E5, 200}, {D5, 200}, {B4, 250}, {G4, 400},
    {B4, 250}, {E5, 200}, {G5, 200}, {E5, 250},
    
    // Next section - mirroring the main theme
    {D5, 200}, {B4, 250}, {G4, 400}, {E4, 400},
    {G4, 400}, {B4, 250}, {E5, 200}, {D5, 200},
    {B4, 250}, {G4, 400}, {B4, 250}, {E5, 200},
    {D5, 200}, {B4, 250}, {G4, 400}, {B4, 250},
    {E5, 200}, {G5, 200}, {E5, 250}, {D5, 200},
    {B4, 250}, {G4, 400},
    
    // End note with decay
    {E4, 250}, {R, 150}, {E4, 300}, {R, 400}
};

// Zelda - Main Theme
static Note zelda_theme[] = {
    // Intro - mystery
    {A4, 240}, {R, 60}, {A4, 120}, {R, 30}, {A4, 120}, {R, 30},
    {A4, 180}, {F4, 60}, {A4, 240}, {R, 120},
    
    // Main theme - heroic section
    {A4, 180}, {A4, 60}, {A4, 240}, {G4, 200}, {F4, 80}, 
    {G4, 160}, {A4, 160}, {D5, 480}, {R, 160},
    
    // Second part of main theme
    {A4, 180}, {A4, 60}, {A4, 240}, {G4, 200}, {F4, 80},
    {G4, 160}, {E4, 160}, {D4, 480}, {R, 160},
    
    // Variation
    {F4, 180}, {F4, 60}, {F4, 240}, {E4, 200}, {D4, 80},
    {E4, 160}, {F4, 160}, {A4, 480}, {R, 160},
    
    // Final phrase
    {D5, 180}, {D5, 60}, {D5, 160}, {A4, 320}, {R, 160},
    {F4, 160}, {G4, 160}, {A4, 320}, {R, 160},
    {D4, 480}, {R, 320}
};

// Tetris Theme (Korobeiniki)
static Note tetris_theme[] = {
    // Main melody - Part 1
    {E5, 120}, {B4, 60}, {C5, 60}, {D5, 120}, {C5, 60}, {B4, 60},
    {A4, 120}, {A4, 60}, {C5, 60}, {E5, 120}, {D5, 60}, {C5, 60},
    {B4, 180}, {C5, 60}, {D5, 120}, {E5, 120},
    {C5, 120}, {A4, 120}, {A4, 240}, {R, 60},
    
    // Main melody - Part 2
    {D5, 120}, {F5, 60}, {A5, 120}, {G5, 60}, {F5, 60},
    {E5, 180}, {C5, 60}, {E5, 120}, {D5, 60}, {C5, 60},
    {B4, 120}, {B4, 60}, {C5, 60}, {D5, 120}, {E5, 120},
    {C5, 120}, {A4, 120}, {A4, 120}, {R, 60},
    
    // Faster section
    {E4, 90}, {C5, 90}, {D5, 90}, {B4, 90}, {C5, 90}, {A4, 90}, {Gs4, 90}, {B4, 180},
    {E4, 90}, {C5, 90}, {D5, 90}, {B4, 90}, {C5, 90}, {E5, 90}, {E5, 90}, {E5, 180},
    
    // Finale
    {C5, 120}, {D5, 120}, {E5, 120}, {C5, 120}, {D5, 120}, {B4, 120}, {C5, 120}, {A4, 240}, 
    {R, 120}, {E5, 120}, {C5, 120}, {A4, 360}, {R, 240}
};


// Super Mario Bros. underworld theme - improved timing
static Note mario_underworld[] = {
    // First section - plucky alternating notes
    {C3, 120}, {R, 30}, {C4, 120}, {R, 30}, {C3, 120}, {R, 30}, {C4, 120}, {R, 30},
    {C3, 120}, {R, 30}, {C4, 120}, {R, 30}, {E3, 120}, {R, 30}, {F4, 120}, {R, 30},
    {G3, 120}, {R, 30}, {A4, 120}, {R, 30}, {B3, 120}, {R, 30}, {C4, 120}, {R, 150},
    
    // Repeat of first section
    {C3, 120}, {R, 30}, {C4, 120}, {R, 30}, {C3, 120}, {R, 30}, {C4, 120}, {R, 30},
    {C3, 120}, {R, 30}, {C4, 120}, {R, 30}, {E3, 120}, {R, 30}, {F4, 120}, {R, 30},
    {G3, 120}, {R, 30}, {A4, 120}, {R, 30}, {B3, 120}, {R, 30}, {C4, 120}, {R, 150},
    
    // Chromatic descent section - smoothed timing
    {C4, 100}, {B3, 100}, {As3, 100}, {A3, 100},
    {G3, 90}, {Fs3, 90}, {F3, 90}, {E3, 90}, {Ds3, 90},
    {D3, 120}, {Cs3, 120}, {C3, 300}, {R, 100}, {C3, 100}, {R, 300}
};

// Creepy Nuts inspired hip-hop melody - improved rhythm
static Note hiphop_melody[] = {
    // Intro - smoother timing and better phrasing
    {D4, 150}, {F4, 150}, {A4, 300}, {R, 30}, {G4, 120}, {A4, 150},
    {F4, 300}, {D4, 150}, {F4, 150}, {G4, 150}, {A4, 150},
    {C5, 300}, {A4, 150}, {G4, 250}, {R, 100},
   
    // First verse rhythm - tightened triplet feeling
    {D4, 80}, {D4, 80}, {D4, 80}, {R, 30}, {F4, 170}, {D4, 100}, {F4, 170},
    {G4, 100}, {A4, 250}, {R, 50}, {G4, 150}, {F4, 150}, {D4, 250},
    {F4, 80}, {F4, 80}, {F4, 80}, {R, 30}, {G4, 170}, {F4, 100}, {G4, 170},
    {A4, 100}, {C5, 250}, {R, 50}, {A4, 150}, {G4, 250}, {R, 100},
   
    // Hook section - smoother transitions
    {D5, 150}, {C5, 150}, {A4, 250}, {R, 50}, {G4, 150}, {A4, 150},
    {G4, 150}, {F4, 250}, {R, 50}, {D4, 150}, {F4, 150}, {G4, 150},
    {A4, 150}, {G4, 150}, {F4, 250}, {R, 50}, {D4, 150}, {F4, 150},
    {G4, 150}, {A4, 250}, {R, 50}, {G4, 150}, {F4, 250}, {R, 50},
   
    // Second verse - faster hi-hat style rhythm
    {D4, 60}, {D4, 60}, {F4, 120}, {R, 30}, {D4, 60}, {D4, 60}, {F4, 120}, {R, 30},
    {G4, 150}, {A4, 150}, {C5, 150}, {A4, 150}, {G4, 250}, {R, 50},
    {F4, 60}, {F4, 60}, {G4, 120}, {R, 30}, {F4, 60}, {F4, 60}, {G4, 120}, {R, 30},
    {A4, 150}, {C5, 150}, {D5, 150}, {C5, 150}, {A4, 250}, {R, 50},
   
    // Outro - cleaner ending
    {D5, 150}, {C5, 150}, {A4, 250}, {R, 50}, {G4, 150}, {A4, 150},
    {F4, 250}, {R, 50}, {D4, 150}, {F4, 150}, {G4, 150}, {A4, 150},
    {G4, 250}, {R, 50}, {F4, 150}, {D4, 300}, {R, 70}, {D4, 80}, {R, 300}
};

// Star Wars Theme
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

// "Take On Me" by A-ha (music_2)
static Note take_on_me[] = {
    // Main melody pattern - improved timing and phrasing
    {A4, 180}, {E5, 180}, {A5, 180}, {R, 60}, {A5, 180}, {A5, 180}, {Gs5, 180}, {A5, 180},
    {R, 60}, {E5, 180}, {R, 60}, {E5, 180}, {R, 60}, {E5, 180}, {R, 60}, {E5, 180},
    
    // Repeat of melody pattern
    {A4, 180}, {E5, 180}, {A5, 180}, {R, 60}, {A5, 180}, {A5, 180}, {Gs5, 180}, {A5, 180},
    {R, 60}, {E5, 180}, {R, 60}, {E5, 180}, {R, 60}, {E5, 180}, {R, 60}, {E5, 180},
    
    // Final repeat with slight variation
    {A4, 180}, {E5, 180}, {A5, 180}, {R, 60}, {A5, 180}, {A5, 180}, {Gs5, 180}, {A5, 180},
    {R, 60}, {E5, 180}, {R, 60}, {E5, 180}, {R, 60}, {E5, 180}, {R, 60}, {E5, 270}, {R, 90}
};

// "Ode to Joy" Beethoven's 9th Symphony (music_3)
static Note ode_to_joy[] = {
    // First phrase - improved timing
    {E4, 180}, {E4, 180}, {F4, 180}, {G4, 180}, {G4, 180}, {F4, 180}, {E4, 180}, {D4, 180},
    {C4, 180}, {C4, 180}, {D4, 180}, {E4, 180}, {E4, 360}, {D4, 90}, {D4, 270},
    
    // Second phrase
    {E4, 180}, {E4, 180}, {F4, 180}, {G4, 180}, {G4, 180}, {F4, 180}, {E4, 180}, {D4, 180},
    {C4, 180}, {C4, 180}, {D4, 180}, {E4, 180}, {D4, 270}, {C4, 90}, {C4, 360}, {R, 90},
    
    // Third phrase
    {D4, 180}, {D4, 180}, {E4, 180}, {C4, 180}, {D4, 180}, {E4, 90}, {F4, 90}, {E4, 180}, {C4, 180},
    {D4, 180}, {E4, 90}, {F4, 90}, {E4, 180}, {D4, 180}, {C4, 180}, {D4, 180}, {G3, 360}, {R, 90}
};

// "Twinkle Twinkle Little Star" (music_4)
static Note twinkle_twinkle[] = {
    // First verse - improved timing
    {C4, 450}, {R, 50}, {C4, 450}, {R, 50}, {G4, 450}, {R, 50}, {G4, 450}, {R, 50},
    {A4, 450}, {R, 50}, {A4, 450}, {R, 50}, {G4, 900}, {R, 100},
    
    {F4, 450}, {R, 50}, {F4, 450}, {R, 50}, {E4, 450}, {R, 50}, {E4, 450}, {R, 50},
    {D4, 450}, {R, 50}, {D4, 450}, {R, 50}, {C4, 900}, {R, 100},
    
    // Second verse
    {G4, 225}, {R, 25}, {G4, 225}, {R, 25}, {F4, 225}, {R, 25}, {F4, 225}, {R, 25}, 
    {E4, 450}, {R, 50}, {C4, 450}, {R, 50},
    
    {G4, 225}, {R, 25}, {G4, 225}, {R, 25}, {F4, 225}, {R, 25}, {F4, 225}, {R, 25}, 
    {E4, 450}, {R, 50}, {C4, 450}, {R, 50},
    
    // Final line
    {C4, 450}, {R, 50}, {G3, 450}, {R, 50}, {C4, 900}, {R, 100}
};

#endif
