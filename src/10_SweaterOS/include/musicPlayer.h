#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "libc/stdint.h"
#include "frequencies.h"

// Musical note frequencies (in Hz)
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523

// Forward declare structures
struct Note;
struct Song;
struct SongPlayer;

// Note structure
typedef struct {
    uint32_t frequency;  // Frequency in Hz
    uint32_t duration;   // Duration in milliseconds
} Note;

// Song structure
typedef struct {
    Note* notes;        // Pointer to array of notes
    uint32_t length;    // Number of notes in the song
} Song;

// SongPlayer structure
typedef struct {
    void (*play_song)(Song* song); // Function pointer to play_song implementation
} SongPlayer;

/**
 * Create a new song player instance
 * @return Pointer to the new SongPlayer instance
 */
SongPlayer* create_song_player(void);

/**
 * Create a new note
 * @param frequency The frequency of the note in Hz
 * @param duration The duration of the note in milliseconds
 * @return Pointer to the new Note instance
 */
Note* create_note(uint32_t frequency, uint32_t duration);

/**
 * Create a new song
 * @param notes Array of notes
 * @param length Number of notes in the song
 * @return Pointer to the new Song instance
 */
Song* create_song(Note* notes, uint32_t length);

/**
 * Free a song and its resources
 * @param song Pointer to the Song instance to free
 */
void free_song(Song* song);

/**
 * Play a song
 * @param song Pointer to the Song structure to play
 */
void play_song(Song* song);

/**
 * Implementation of play_song that handles the actual playing of notes
 * @param song Pointer to the Song structure to play
 */
void play_song_impl(Song* song);

/**
 * Free a song player instance
 * @param player Pointer to the SongPlayer instance to free
 */
void free_song_player(SongPlayer* player);

// Predefined melodies (shortened versions)
static Note mario_melody[] = {
    {E5, 200}, {E5, 200}, {E5, 200},
    {C5, 200}, {E5, 200}, {G5, 400},
    {G4, 400}
};

static Note twinkle_melody[] = {
    {C4, 400}, {C4, 400}, {G4, 400}, {G4, 400},
    {A4, 400}, {A4, 400}, {G4, 800}
};

static Note jingle_bells[] = {
    {E4, 300}, {E4, 300}, {E4, 600},
    {E4, 300}, {E4, 300}, {E4, 600}
};

static Note imperial_march[] = {
    {F4, 250}, {F4, 250}, {F4, 250},
    {C5, 250}, {A_SHARP4, 250}, {F4, 500}
};

#endif /* MUSIC_PLAYER_H */ 