#include "musicPlayer.h"
#include "display.h"
#include "pcSpeaker.h"
#include "memory_manager.h"
#include "miscFuncs.h"
#include "programmableIntervalTimer.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

/**
 * Create a new song player instance
 */
SongPlayer* create_song_player(void) {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    if (player) {
        player->play_song = play_song_impl;
    }
    return player;
}

/**
 * Implementation of play_song that handles the actual playing of notes
 */
void play_song_impl(Song* song) {
    if (!song || !song->notes || song->length == 0) {
        display_write_color("Invalid song data\n", COLOR_RED);
        return;
    }

    display_write_color("Playing song...\n", COLOR_GREEN);
    
    // Turn off speaker initially to ensure clean start
    disable_speaker();
    
    // Play each note in sequence
    for (uint32_t i = 0; i < song->length; i++) {
        const Note* current_note = &song->notes[i];
        
        // Enklere output med mindre detaljert informasjon
        display_write_color("Playing note: ", COLOR_GREEN);
        
        // Vis note-navn i stedet for frekvens
        char* note_name = "?";
        if (current_note->frequency == NOTE_C4) note_name = "C4";
        else if (current_note->frequency == NOTE_CS4) note_name = "C#4";
        else if (current_note->frequency == NOTE_D4) note_name = "D4";
        else if (current_note->frequency == NOTE_DS4) note_name = "D#4";
        else if (current_note->frequency == NOTE_E4) note_name = "E4";
        else if (current_note->frequency == NOTE_F4) note_name = "F4";
        else if (current_note->frequency == NOTE_FS4) note_name = "F#4";
        else if (current_note->frequency == NOTE_G4) note_name = "G4";
        else if (current_note->frequency == NOTE_GS4) note_name = "G#4";
        else if (current_note->frequency == NOTE_A4) note_name = "A4";
        else if (current_note->frequency == NOTE_AS4) note_name = "A#4";
        else if (current_note->frequency == NOTE_B4) note_name = "B4";
        else if (current_note->frequency == NOTE_C5) note_name = "C5";
        else if (current_note->frequency == 0) note_name = "REST";
        
        display_write_color(note_name, COLOR_GREEN);
        display_write_color("\n", COLOR_GREEN);
        
        // Play the note with direct control for better performance
        if (current_note->frequency > 0) {
            // Calculate divisor
            uint16_t divisor = 1193180 / current_note->frequency;
            
            // Disable interrupts while configuring sound
            __asm__ volatile("cli");
            
            // Configure PIT channel 2
            outb(0x43, 0xB6);
            outb(0x42, divisor & 0xFF);
            outb(0x42, (divisor >> 8) & 0xFF);
            
            // Enable speaker
            outb(0x61, inb(0x61) | 0x03);
            
            // Re-enable interrupts
            __asm__ volatile("sti");
        } else {
            // Turn off speaker for rest
            uint8_t tmp = inb(0x61);
            outb(0x61, tmp & ~0x03);
        }
        
        // Wait for the duration of the note
        // Use sleep_interrupt for longer notes and sleep_busy for very short notes
        uint32_t duration = current_note->duration;
        if (duration >= 20) {
            sleep_interrupt(duration);
        } else {
            sleep_busy(duration);
        }
        
        // Stop the sound explicitly
        uint8_t tmp = inb(0x61);
        outb(0x61, tmp & ~0x03);
        
        // Add a very small gap between notes (5ms) for better sound separation
        sleep_busy(5);
    }
    
    // Ensure speaker is off at the end
    disable_speaker();
    
    display_write_color("Song finished\n", COLOR_GREEN);
}

/**
 * Play a song using the default implementation
 */
void play_song(Song* song) {
    play_song_impl(song);
}

/**
 * Free a song player instance
 */
void free_song_player(SongPlayer* player) {
    if (player) {
        free(player);
    }
}

/**
 * Free song resources
 */
void free_song(Song* song) {
    if (song) {
        if (song->notes) {
            free(song->notes);
        }
        free(song);
    }
}

/**
 * Create a new note
 */
Note* create_note(uint32_t frequency, uint32_t duration) {
    Note* note = (Note*)malloc(sizeof(Note));
    if (note) {
        note->frequency = frequency;
        note->duration = duration;
    }
    return note;
}

/**
 * Create a new song
 */
Song* create_song(Note* notes, uint32_t length) {
    Song* song = (Song*)malloc(sizeof(Song));
    if (song) {
        song->notes = notes;
        song->length = length;
    }
    return song;
} 