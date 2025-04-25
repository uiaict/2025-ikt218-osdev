#include <stdint.h>
#include "pit.h"
#include "song/song.h"
#include "libc/stdio.h"

// PC speaker port
#define PC_SPEAKER_PORT 0x61
#define PIT_CHANNEL2 0x42
#define PIT_COMMAND 0x43

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void enable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 3); // set bits 0 and 1
}

void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC); // clear bits 0 and 1
}

void play_sound(uint32_t freq) {
    if (freq == 0) return;

    uint32_t divisor = 1193182 / freq;

    outb(PIT_COMMAND, 0xB6); // channel 2, mode 3 (square wave), lobyte/hibyte
    outb(PIT_CHANNEL2, (uint8_t)(divisor & 0xFF));        // low byte
    outb(PIT_CHANNEL2, (uint8_t)((divisor >> 8) & 0xFF)); // high byte

    enable_speaker();
}

void stop_sound() {
    disable_speaker();
}

// 🔧 Her er den manglende funksjonen:
void play_song_impl(Song* song) {
    for (size_t i = 0; i < song->note_count; i++) {
        Note note = song->notes[i];
        printf("Note: %u Hz, duration: %u ms\n", note.frequency, note.duration_ms);
        play_sound(note.frequency);
        for (volatile uint32_t j = 0; j < note.duration_ms * 1000; j++) {} // Fake sleep'
        stop_sound();
        printf("Note: %d Hz, duration: %d ms\n", (int)note.frequency, (int)note.duration_ms);

    }
}
