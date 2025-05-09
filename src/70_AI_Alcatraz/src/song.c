#include "song.h"
#include "pit.h"
#include "printf.h"
#include "memory.h"   // For malloc, free

// Helper function to read byte from port (copied from printf.h)
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// --- Sample Song Definitions ---

// Mario theme intro notes
#define NOTE_E7 2637
#define NOTE_G6 1568
#define NOTE_C7 2093
#define NOTE_E6 1319
#define NOTE_A6 1760
#define NOTE_B6 1976
#define REST 0 // Represents silence

Note music_1[] = {
    {E5, 250}, {E5, 250}, {R, 250}, {E5, 250}, 
    {R, 250}, {C5, 250}, {E5, 250}, {R, 250},
    {G5, 500}, {R, 500}, {G4, 500}, {R, 500},
    
    // More notes can be added here
};
uint32_t music_1_length = sizeof(music_1) / sizeof(Note);

// Enable the PC Speaker
void enable_speaker() {
    // Read the current state from the PC speaker control port
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    
    // Check if bits 0 and 1 are not set (0 means that the speaker is disabled)
    if ((speaker_state & 3) != 3) {
        // Enable the speaker by setting bits 0 and 1 to 1
        outb(PC_SPEAKER_PORT, speaker_state | 3);
    }
}

// Disable the PC Speaker
void disable_speaker() {
    // Turn off the PC speaker by clearing bits 0 and 1
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, speaker_state & 0xFC); // 0xFC = 11111100b
}

// Play a sound with the given frequency
void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        // If frequency is 0, ensure speaker is off (rest)
        disable_speaker(); 
        return;
    }

    // Calculate the PIT divisor
    uint16_t divisor = (uint16_t)(PIT_BASE_FREQUENCY / frequency);

    // Configure PIT channel 2 for sound generation
    // 0xB6 = 10110110 (channel 2, access mode: lobyte/hibyte, mode 3: square wave)
    outb(PIT_CMD_PORT, 0xB6);
    
    // Send divisor (low byte, then high byte)
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor >> 8));

    // Enable the speaker *after* setting the frequency
    enable_speaker();
}

// Stop sound generation
void stop_sound() {
    disable_speaker();
}

// Play an entire song
void play_song_impl(Song* song) {
    if (!song || !song->notes) {
        printf("Invalid song or note data\n");
        return;
    }

    printf("Playing song with %d notes\n", song->length);
    
    for (uint32_t i = 0; i < song->length; i++) {
        Note* note = &song->notes[i];
        
        // Set the frequency for the current note.
        // play_sound handles enabling the speaker for freq > 0
        // and disabling it for freq == 0 (rests).
        play_sound(note->frequency);
        
        // Hold the note (or rest) for its duration.
        sleep_interrupt(note->duration);
        
        // Add a small gap *after* the note's duration has passed.
        // During this gap, the speaker should be off.
        stop_sound(); // Turn speaker off explicitly during the gap
        sleep_interrupt(30); 
    }
    
    // Ensure speaker is off at the very end of the song.
    stop_sound();
}

// Public-facing function to play a song
void play_song(Song* song) {
    play_song_impl(song);
}

// Create a new song player
SongPlayer* create_song_player() {
    SongPlayer* player = malloc(sizeof(SongPlayer));
    if (player) {
        player->play_song = play_song;
    }
    return player;
}
