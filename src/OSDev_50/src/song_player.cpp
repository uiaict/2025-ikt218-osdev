extern "C" {
    #include "song.h"
    #include "pit.h"
    #include "common.h"    // for inb/outb
    #include "libc/stdio.h"
    #include <stdint.h>
}

// PC Speaker and PIT ports
#define PIT_CONTROL_PORT 0x43
#define PIT_CHANNEL2_PORT 0x42
#define SPEAKER_CTRL_PORT 0x61
#define PIT_BASE_FREQ 1193182U

extern "C" uint8_t inb(uint16_t port);
extern "C" void outb(uint16_t port, uint8_t value);

extern "C" {

// Enable PC speaker by setting bits 0 (gate) and 1 (data) on port 0x61
void enable_speaker() {
    uint8_t state = inb(SPEAKER_CTRL_PORT);
    state |= 0x03;               // set bits 0 and 1
    outb(SPEAKER_CTRL_PORT, state);
}

// Disable PC speaker by clearing bits 0 and 1
void disable_speaker() {
    uint8_t state = inb(SPEAKER_CTRL_PORT);
    state &= ~0x03;              // clear bits 0 and 1
    outb(SPEAKER_CTRL_PORT, state);
}

// Silence PC speaker by clearing the speaker-enable bit (bit 0)
void stop_sound() {
    uint8_t state = inb(SPEAKER_CTRL_PORT);
    state &= ~0x01;              // clear bit 0 instead of bit 1
    outb(SPEAKER_CTRL_PORT, state);
}

// Play a tone at the given frequency (Hz)
void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        stop_sound();
        return;
    }

    // Calculate PIT divisor
    uint32_t divisor = PIT_BASE_FREQ / frequency;
    uint8_t low  = divisor & 0xFF;
    uint8_t high = (divisor >> 8) & 0xFF;

    // Configure PIT: channel 2, square wave (mode 3), lobyte/hibyte
    outb(PIT_CONTROL_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, low);
    outb(PIT_CHANNEL2_PORT, high);

    // Turn on speaker
    enable_speaker();
}

// Play all notes in a song
void play_song_impl(Song* song) {
    if (!song || !song->notes || song->note_count == 0)
        return;

    for (size_t i = 0; i < song->note_count; ++i) {
        Note n = song->notes[i];
        printf("Note %u: freq=%u Hz, dur=%u ms\n", i, n.frequency, n.duration_ms);
        play_sound(n.frequency);
        sleep_busy(n.duration_ms);    // Uses PIT for ms delay
        stop_sound();
        sleep_busy(10);               // brief pause between notes
    }
}

// Wrapper to match SongPlayer interface
void play_song(Song* song) {
    play_song_impl(song);
}

// Ensure enable/disable of speaker before and after full song
void play_song_safe(Song* song) {
    enable_speaker();
    play_song_impl(song);
    disable_speaker();
}

} // extern "C"
