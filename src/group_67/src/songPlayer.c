
#include "libc/util.h"    
#include "libc/PitTimer.h"   
#include "libc/stdio.h"
#include "libc/song.h"

void enableSpeaker() {
    uint8_t tmp = inPortB(PC_SPEAKER_PORT);
    outPortB(PC_SPEAKER_PORT, tmp | 0x03); // Set bits 0 and 1 to enable speaker
}

void disableSpeaker() {
    uint8_t tmp = inPortB(PC_SPEAKER_PORT);
    outPortB(PC_SPEAKER_PORT, tmp & ~0x03); // Clear bits 0 and 1 to disable
}

void playSound(uint32_t frequency) {
    if (frequency == 0) return;

    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;

    outPortB(PIT_CMD_PORT, 0xB6); // PIT command byte: channel 2, mode 3, binary
    outPortB(PIT_CHANNEL2_PORT, divisor & 0xFF);       // Low byte
    outPortB(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF); // High byte

    enableSpeaker();
}

void stopSound() {
    uint8_t port_val = inPortB(PC_SPEAKER_PORT);
    port_val &= ~0x02;                     // Clear bit 1 (speaker data)
    outPortB(PC_SPEAKER_PORT, port_val);             
}



void play_song_impl(Song *song) {

    enableSpeaker();

    // Step 2: Loop through each note in the song's notes array
    for (uint32_t i = 0; i < song->length; ++i) {

        playSound(song->notes[i].frequency);

        sleep_busy(song->notes[i].duration);

        stopSound();
    }

    disableSpeaker();
}

// Implementing the play_song function
void playSong(Note *music, uint32_t countBits) {
    uint32_t note_count = countBits / sizeof(Note);
    Song song = {
        .notes = music,
        .length = note_count
    };

    play_song_impl(&song);
}