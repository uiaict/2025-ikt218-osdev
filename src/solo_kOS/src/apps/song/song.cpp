extern "C" {
    #include "apps/song/song.h"
    #include "kernel/pit.h"
    #include "common/io.h"
    #include "kernel/interrupts.h"
    #include "libc/stdio.h"   
}

void enable_speaker() {
    uint8_t value = inb(PC_SPEAKER_PORT);
    value |= 0x03; // Set bit 0 (gate) and bit 1 (data)
    outb(PC_SPEAKER_PORT, value);
}

void disable_speaker() {
    uint8_t value = inb(PC_SPEAKER_PORT);
    value &= ~0x03; // Clear bit 0 and bit 1
    outb(PC_SPEAKER_PORT, value);
}

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

void stop_sound() {
    uint8_t value = inb(PC_SPEAKER_PORT);
    value &= ~0x02; // Clear bit 1 (speaker data), keep PIT gate (bit 0) unchanged
    outb(PC_SPEAKER_PORT, value);
}

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

void play_song(Song* song) {
    play_song_impl(song);
}

SongPlayer* create_song_player() {
    SongPlayer* player = new SongPlayer;
    player->play_song = play_song;
    return player;
}