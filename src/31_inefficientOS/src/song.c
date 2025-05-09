#include "song.h"
#include "terminal.h"
#include "common.h"
#include "pit.h"
#include "libc/stdio.h"

// Define hardware ports for PC Speaker
#define SPEAKER_PORT 0x61
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL2_PORT 0x42
#define PIT_FREQUENCY 1193182 // Base frequency in Hz

/**
 * Enable the PC speaker by setting the appropriate bits in the control port.
 */
static void enable_speaker() {
    uint8_t tmp = inb(SPEAKER_PORT);
    // Set bits 0 and 1 to enable the speaker and connect timer to speaker
    outb(SPEAKER_PORT, tmp | 0x3);
}

/**
 * Disable the PC speaker by clearing the appropriate bits in the control port.
 */
static void disable_speaker() {
    uint8_t tmp = inb(SPEAKER_PORT);
    // Clear bits 0 and 1 to disable the speaker
    outb(SPEAKER_PORT, tmp & ~0x3);
}

/**
 * Play a sound of the specified frequency through the PC speaker.
 * 
 * @param frequency The frequency in Hz to play (0 means no sound)
 */
static void play_sound(uint32_t frequency) {
    // If frequency is 0, don't play any sound (rest)
    if (frequency == 0) {
        // For rests, we just silence the speaker but don't disable it
        uint8_t tmp = inb(SPEAKER_PORT);
        outb(SPEAKER_PORT, tmp & ~0x2); // Clear bit 1 to stop sound
        return;
    }

    // Calculate the divisor for the PIT
    uint32_t divisor = PIT_FREQUENCY / frequency;
    
    // Configure PIT channel 2 in mode 3 (square wave generator)
    // 0xB6 = 10110110 in binary
    // Bits 7-6: 10 = Channel 2
    // Bits 5-4: 11 = Access mode: low byte then high byte
    // Bits 3-1: 011 = Mode 3 (square wave generator)
    // Bit 0: 0 = Binary mode (16-bit)
    outb(PIT_COMMAND_PORT, 0xB6);
    
    // Send the divisor (low byte then high byte)
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);         // Low byte
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);  // High byte
    
    // Enable the speaker if it's not already enabled
    uint8_t tmp = inb(SPEAKER_PORT);
    if ((tmp & 0x3) != 0x3) {
        outb(SPEAKER_PORT, tmp | 0x3);
    }
}

/**
 * Implementation of the song player that plays each note in sequence.
 * 
 * @param song Pointer to the Song structure containing notes to play
 */
void play_song_impl(Song *song) {
    if (!song || !song->notes || song->length == 0) {
        terminal_writestring("Invalid song data\n");
        return;
    }

    // Enable the speaker once at the beginning
    enable_speaker();
    
    // Play each note in sequence
    for (uint32_t i = 0; i < song->length; i++) {
        Note note = song->notes[i];
        
        // Play the current note (or rest if frequency is 0)
        play_sound(note.frequency);
        
        // Hold the note for its duration
        sleep_interrupt(note.duration);
        
        // For legato (smooth) effect, we don't turn off the speaker between notes
        // Instead, we just change the frequency directly
        
        // If this is the last note, make sure to turn off the speaker
        if (i == song->length - 1) {
            disable_speaker();
        }
    }
    
    terminal_writestring("Song finished\n");
}

/**
 * Function that will be attached to the SongPlayer struct
 * to play a song.
 * 
 * @param song Song structure to play
 */
static void song_player_play_song(Song song) {
    play_song_impl(&song);
}

/**
 * Create a new SongPlayer instance.
 * 
 * @return Pointer to a newly created SongPlayer instance
 */
SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    if (!player) {
        terminal_writestring("Failed to allocate memory for SongPlayer\n");
        return NULL;
    }
    
    player->play_song = song_player_play_song;
    return player;
}