#ifndef SONG_PLAYER_H
#define SONG_PLAYER_H

#include "libc/stdint.h"

// Function prototypes
void play_sound(uint32_t frequency);   // Function to play sound with a given frequency
void delay(uint32_t duration);         // Function to introduce a delay (in some units like ms)
void stop_sound();                     // Function to stop the sound

// Changed from uint8_t to uint16_t to handle larger frequency values
extern const uint16_t example_song[];

// Function to play a song from song data
void play_song(const uint16_t* song_data);

#endif // SONG_PLAYER_H
