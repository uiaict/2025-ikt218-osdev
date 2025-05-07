#ifndef SONG_H
#define SONG_H

#include "libc/stdint.h"
#include "libc/stdbool.h"

// PIT and speaker constants
#define PC_SPEAKER_PORT 0x61
#define PIT_BASE_FREQUENCY 1193180  // Hz
#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL2_PORT 0x42

// Musical note frequencies
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
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
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988

// Data structures
typedef struct {
    uint32_t frequency;  // Frequency in Hz
    uint32_t duration;   // Duration in milliseconds
} Note;

typedef struct {
    Note *notes;
    uint32_t length;
} Song;

typedef struct {
    void (*play_song)(Song *song);
} SongPlayer;

// Function declarations
SongPlayer* create_song_player(void);

// Background music control functions
void start_background_music(Song *song, bool loop);
void stop_background_music(void);
void song_tick(uint32_t tick_ms);

// Low-level sound control functions
void speaker_control(bool on);
void set_frequency(uint32_t hz);
void delay_ms(uint32_t ms);

#endif