#ifndef SONG_H
#define SONG_H

#include "libc/stddef.h"
#include "libc/stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// PC Speaker control ports
#define PC_SPEAKER_PORT 0x61
#define PIT_CMD_PORT 0x43
#define PIT_CHANNEL2_PORT 0x42
#define PIT_BASE_FREQUENCY 1193180

// Musical note frequencies
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

// Represents a single musical note
typedef struct {
    uint32_t frequency;
    uint32_t duration;
} Note;

// Represents a song
typedef struct {
    Note* notes;
    size_t length;  // Changed from note_count to match teacher's solution
} Song;

// Song player interface
typedef struct SongPlayer {
    void (*play_song)(Song* song);
} SongPlayer;

// Function declarations
void play_sound(uint32_t frequency);
void stop_sound(void);
void enable_speaker(void);
void disable_speaker(void);
void play_song_impl(Song* song);
SongPlayer* create_song_player(void);

#ifdef __cplusplus
}
#endif

#endif // SONG_H