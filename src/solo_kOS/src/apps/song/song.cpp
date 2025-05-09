extern "C" {
    #include "apps/song/song.h"
    #include "kernel/pit.h"
    #include "common/io.h"
    #include "kernel/interrupts.h"
    #include "libc/stdio.h"   
}

// Function to enable the PC speaker
// This function sets the gate bit (bit 0) and data bit (bit 1) in the PC speaker port
void enable_speaker() {
    uint8_t value = inb(PC_SPEAKER_PORT);
    value |= 0x03; // Set bit 0 (gate) and bit 1 (data)
    outb(PC_SPEAKER_PORT, value);
}

// Function to disable the speaker
// This function clears the gate bit (bit 0) and data bit (bit 1) in the PC speaker port
void disable_speaker() {
    uint8_t value = inb(PC_SPEAKER_PORT);
    value &= ~0x03; // Clear bit 0 and bit 1
    outb(PC_SPEAKER_PORT, value);
}

// Function to play a sound
// This function takes a frequency and plays it through the PC speaker
// The frequency is used to calculate the divisor for the PIT
void play_sound(uint32_t frequency) {
    if (frequency == 0) return;

    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;

    // Command byte: Channel 2, Access mode: lobyte/hibyte, Mode 3 (square wave), Binary mode
    outb(PIT_CMD_PORT, 0xB6);

    // Send divisor to channel 2: low byte first, then high byte
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF));

    enable_speaker();
}

// Function to stop the sound
// This function clears the speaker gate bit in the PC speaker port
void stop_sound() {
    uint8_t value = inb(PC_SPEAKER_PORT);
    value &= ~0x02; // Clear bit 1 (speaker data), keep PIT gate (bit 0) unchanged
    outb(PC_SPEAKER_PORT, value);
}

// Function to play a song
// This function iterates through the notes in the song and plays them
void play_song_impl(Song* song) {
    enable_speaker(); // Speaker on for the whole song

    for (uint32_t i = 0; i < song->length; i++) {
        Note note = song->notes[i]; // Access the current note directly


        // Only play if frequency is not zero (R = rest)
        if (note.frequency > 0) {
            play_sound(note.frequency);
        }

        // Wait for the note duration
        sleep_interrupt(note.duration);

        // Stop the sound after each note or rest
        stop_sound();
    }

    disable_speaker(); // Turn speaker off after song
}

// Function to play a song
void play_song(Song* song) {
    play_song_impl(song);
}

// Function to create a new SongPlayer instance
// This function allocates memory for a SongPlayer struct and initializes it
SongPlayer* create_song_player() {
    SongPlayer* player = new SongPlayer;
    player->play_song = play_song;
    return player;
}