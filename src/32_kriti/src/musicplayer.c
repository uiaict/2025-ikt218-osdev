#include "musicplayer.h"
#include "memory.h"    // For custom malloc/free.
#include "kprint.h"    // For kprint, kprint_dec, and kprint_hex.
#include "pit.h"       // For sleep_interrupt.
#include "isr.h"

// Define the I/O ports for the PIT and PC speaker.
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL2_PORT 0x42
#define SPEAKER_PORT     0x61

// Turn on the tone for a given frequency.
static void tone_on(uint32_t frequency) {
    // The PIT runs at 1,193,180 Hz.
    uint32_t divisor = 1193180 / frequency;

    // Configure PIT channel 2 in square wave mode.
    outb(PIT_COMMAND_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);          // low byte
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);     // high byte

    // Enable the speaker by setting bits 0 and 1.
    uint8_t tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp | 3);
}

// Turn off the tone.
static void tone_off(void) {
    uint8_t tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp & ~3);
}

// play_song_impl iterates over the song's notes. For each note,
// it outputs note information via kprint and then plays an audible tone.
void play_song_impl(Song* song) {
    for (size_t i = 0; i < song->note_count; i++) {
        // Print information about the note.
        kprint("Playing note ");
        kprint_dec(i);
        kprint(" - Frequency: ");
        kprint_dec(song->notes[i].freq);
        kprint(" Hz, Duration: ");
        kprint_dec(song->notes[i].duration);
        kprint(" ms\n");

        // If frequency > 0, play the note.
        if (song->notes[i].freq > 0) {
            tone_on(song->notes[i].freq);
        }

        // Wait for the duration of the note.
        sleep_interrupt(song->notes[i].duration);

        // Turn off the tone.
        tone_off();

        // Optional: add a brief pause between notes.
        sleep_interrupt(50);
    }
}

// create_song_player uses the kernel's malloc to allocate a SongPlayer
// and sets up its play_song function pointer.
SongPlayer* create_song_player(void) {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    if (player) {
        player->play_song = play_song_impl;
    }
    return player;
}
