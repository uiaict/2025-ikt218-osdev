#ifndef SONG_H
#define SONG_H
#include "libc/stdint.h"
#include "./frequencies.h"

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
void run_song_menu();
void stop_sound();
void disable_speaker();
void play_song(Song* song);


// External songs (defined in song.c)
extern Song song_food;
extern Song song_open;
extern Song song_confirm;
extern Song song_fail;
extern Song song_pause;
extern Song song_music_1;
extern Song song_music_2;
extern Song song_music_3;
extern Song song_music_4;
extern Song song_music_5;
extern Song song_music_6;
extern Song song_starwars;

#endif
