#include "song.h"
#include "pit.h"
#include "io.h"
#include "libc/stdio.h"
#include "terminal.h"

#define PC_SPEAKER_PORT 0x61
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL2_PORT 0x42
#define PIT_BASE_FREQUENCY 1193180

// Function to enable PC speaker
static void enable_speaker(void) {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 3);  // Enable bits 0 and 1
}

// Function to disable PC speaker
static void disable_speaker(void) {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC);  // Disable bits 0 and 1
}

// Function to set speaker frequency
void set_speaker_frequency(uint32_t frequency) {
    if (frequency == 0) {
        // Turn off the speaker
        uint8_t tmp = inb(PC_SPEAKER_PORT);
        outb(PC_SPEAKER_PORT, tmp & 0xFC);  // Clear bits 0 and 1
        printf("Speaker disabled\n");
        return;
    }

    // Calculate divisor
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;
    if (divisor > 0xFFFF) divisor = 0xFFFF;  // Clamp to maximum
    
    printf("Setting frequency %u Hz (divisor: %u)\n", frequency, divisor);

    // Prepare speaker - disable it first
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC);

    // Configure channel 2 for square wave generation
    outb(PIT_COMMAND_PORT, 0xB6);    // 10110110 - Channel 2, square wave mode
    
    // Set frequency divisor
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));        // Low byte
    outb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    // Now enable the speaker
    tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 0x03);  // Set bits 0 and 1 (enable speaker + timer 2)
    
    printf("Speaker enabled (port 0x61 value: 0x%02x)\n", inb(PC_SPEAKER_PORT));
}

// Implementation of play_song function
static void play_song_impl(Song* song) {
    if (!song || !song->notes || song->note_count == 0) {
        return;
    }

    for (size_t i = 0; i < song->note_count; i++) {
        Note current_note = song->notes[i];       
        printf("Playing frequency %u Hz for %u ms\n", 
               current_note.frequency, 
               current_note.duration);
        if (current_note.frequency > 0) {
            set_speaker_frequency(current_note.frequency);
            sleep_busy(current_note.duration);
            disable_speaker();
        } else {
            // This is a rest - just wait
            sleep_busy(current_note.duration);
        }
        // Small gap between notes
        sleep_busy(50);
    }

    // Ensure speaker is off when done
    disable_speaker();
}

// Static SongPlayer instance
static SongPlayer player = {
    .play_song = play_song_impl
};

// Create and return a SongPlayer instance
SongPlayer* create_song_player(void) {
    return &player;
}