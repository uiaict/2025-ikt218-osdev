#ifndef SONG_H
#define SONG_H

#include "libc/stdint.h"
#include "libc/stddef.h"

// Define note frequencies
#define R 0      // Rest (no sound)
#define C3 131
#define CS3 139
#define D3 147
#define DS3 156
#define E3 165
#define F3 175
#define FS3 185
#define G3 196
#define GS3 208
#define A3 220
#define AS3 233
#define B3 247

#define C4 262
#define CS4 277
#define D4 294
#define DS4 311
#define E4 330
#define F4 349
#define FS4 370
#define G4 392
#define GS4 415
#define A4 440
#define AS4 466
#define B4 494

#define C5 523
#define CS5 554
#define D5 587
#define DS5 622
#define E5 659
#define F5 698
#define FS5 740
#define G5 784
#define GS5 831
#define A5 880
#define AS5 932
#define B5 988

// A note has a frequency and a duration
typedef struct {
    uint32_t frequency;  // Frequency in Hz
    uint32_t duration;   // Duration in ms
} Note;

// A song is a collection of notes
typedef struct {
    Note* notes;
    uint32_t length;
} Song;

// Song player structure
typedef struct {
    void (*play_song)(Song* song);
} SongPlayer;

// Sample song - Super Mario Bros theme
extern Note music_1[];
extern uint32_t music_1_length;

// Function prototypes
void enable_speaker();
void disable_speaker();
void play_sound(uint32_t frequency);
void stop_sound();
void play_song_impl(Song* song);
void play_song(Song* song);
SongPlayer* create_song_player();

#endif // SONG_H
