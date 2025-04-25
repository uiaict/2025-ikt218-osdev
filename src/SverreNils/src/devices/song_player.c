#include <stdint.h>
#include "pit.h"
#include "song/song.h"
#include "libc/stdio.h"

// PC speaker I/O-porter og PIT-konfig
#define PC_SPEAKER_PORT 0x61
#define PIT_CHANNEL2    0x42
#define PIT_COMMAND     0x43

// Skriv til I/O-port
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Les fra I/O-port
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Aktiver PC-speaker
void enable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp | 3); // Sett bit 0 og 1
}

// Deaktiver PC-speaker
void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC); // Fjern bit 0 og 1
}

// Start tone på ønsket frekvens via PIT
void play_sound(uint32_t freq) {
    if (freq == 0) return;

    uint32_t divisor = 1193182 / freq;

    outb(PIT_COMMAND, 0xB6); // Kanal 2, mode 3, low/high byte
    outb(PIT_CHANNEL2, (uint8_t)(divisor & 0xFF));        // lav byte
    outb(PIT_CHANNEL2, (uint8_t)((divisor >> 8) & 0xFF)); // høy byte

    enable_speaker();
}

// Stopp lyden
void stop_sound() {
    disable_speaker();
}

// Spill sang note for note
void play_song_impl(Song* song) {
    for (size_t i = 0; i < song->note_count; i++) {
        Note note = song->notes[i];
        printf("Note: %d Hz, duration: %d ms\n", (int)note.frequency, (int)note.duration_ms);

        if (note.frequency > 0) {
            play_sound(note.frequency);
        } else {
            stop_sound(); // eksplisitt stopp ved pause
        }

        sleep_interrupt(note.duration_ms);

        stop_sound(); // ekstra sikkerhet: alltid avslutt lyden etter hver note
    }

    // Just in case: full stopp etter hele sangen
    stop_sound();
}
