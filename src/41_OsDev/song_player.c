// Modified song_player.c for better Star Wars theme playback
#include <libc/stdint.h>
#include <libc/stdio.h>
#include <kernel/pit.h>
#include <kernel/common.h>  // This already contains inb and outb functions
#include <song/song.h>
#include <kernel/memory.h>
#include "terminal.h"

// Function to enable PC speaker
void enable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 3);  // Set bits 0 and 1
}

// Function to disable PC speaker
void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC);  // Clear bits 0 and 1
}

// Function to play a sound
void play_sound(uint32_t frequency) {
    if (frequency == 0) return;  // Rest - no sound
    
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;
    
    // Set PIT channel 2 to mode 3 (square wave generator)
    outb(PIT_CMD_PORT, 0xB6);
    
    // Set the frequency
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);
    
    // Enable the speaker
    enable_speaker();
}

// Function to stop sound
void stop_sound() {
    disable_speaker();
}

// Improved song player implementation
void play_song_impl(Song *song) {
    printf("Playing Star Wars theme...\n");
    
    // Add a short delay before starting the music
    sleep_interrupt(300);
    
    for (size_t i = 0; i < song->length; i++) {
        // Play each note
        play_sound(song->notes[i].frequency);
        
        // Hold the note for its duration
        sleep_interrupt(song->notes[i].duration);
        
        // Brief silence between notes to make them more distinct
        stop_sound();
        sleep_interrupt(25);  // Small gap between notes for Star Wars theme
    }
    
    printf("Song finished.\n");
}

// Create a song player instance
SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song_impl;
    return player;
}