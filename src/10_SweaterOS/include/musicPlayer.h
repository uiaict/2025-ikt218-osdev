#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "libc/stdint.h"
#include "frequencies.h"

// Forhåndsdeklarasjon av strukturer
struct Note;
struct Song;
struct SongPlayer;

/**
 * Note struktur
 * 
 * Representerer en enkelt tone med frekvens og varighet
 */
typedef struct {
    uint32_t frequency;  // Frekvens i Hz
    uint32_t duration;   // Varighet i millisekunder
} Note;

/**
 * Sang struktur
 * 
 * Representerer en sang som en sekvens av noter
 */
typedef struct {
    Note* notes;        // Peker til array av noter
    uint32_t length;    // Antall noter i sangen
} Song;

/**
 * Sangavspiller struktur
 * 
 * Håndterer avspilling av sanger
 */
typedef struct {
    void (*play_song)(Song* song); // Funksjonspeker til play_song implementasjon
} SongPlayer;

/**
 * Sangavspiller funksjoner
 * 
 * Funksjoner for å håndtere sanger og avspilling
 */
SongPlayer* create_song_player(void);           // Oppretter ny sangavspiller
Note* create_note(uint32_t frequency, uint32_t duration); // Oppretter ny note
Song* create_song(Note* notes, uint32_t length); // Oppretter ny sang
void free_song(Song* song);                     // Frigjør minne for sang
void play_song(Song* song);                     // Spiller av sang
void play_song_impl(Song* song);                // Intern implementasjon av avspilling
void free_song_player(SongPlayer* player);      // Frigjør minne for sangavspiller

// Forhåndsdefinerte melodier
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
    {C4, 500}, {G3, 500}, {C4, 1000},
    {C4, 500}, {G3, 500}, {C4, 1000}
};

#endif /* MUSIC_PLAYER_H */ 