extern "C" {
    #include "libc/stdio.h"
    #include "libc/stdint.h"
    #include "memory.h"
    #include "song.h"
    #include "pit.h"
}

// === I/O functions ===
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// === Speaker control ===
void enable_speaker() {
    uint8_t val = inb(0x61);
    if ((val & 0x03) != 0x03) {
        outb(0x61, val | 0x03);  // Enable speaker gate and data
    }
}

void disable_speaker() {
    uint8_t val = inb(0x61) & 0xFC;  // Clear bits 0 and 1
    outb(0x61, val);
}

// === Sound ===
void play_sound(uint32_t frequency) {
    if (frequency == 0) return;

    uint32_t divisor = 1193180 / frequency;

    // Send command byte to PIT (channel 2, mode 3, binary)
    outb(0x43, 0xB6);
    outb(0x42, divisor & 0xFF);         // Low byte
    outb(0x42, (divisor >> 8) & 0xFF);  // High byte

    enable_speaker();
}

void stop_sound() {
    disable_speaker();
}

// === Timing ===
extern volatile uint32_t pit_ticks;

// === Music playback ===
static void play_song_impl(Song *song) {
    if (!song || song->length == 0) return;

    enable_speaker();
    for (uint32_t i = 0; i < song->length; ++i) {
        Note n = song->notes[i];
        //printf("Note %u: freq=%u dur=%u\n", i, n.frequency, n.duration_ms);
        play_sound(n.frequency);
        sleep_busy(n.duration_ms);
        stop_sound();
    }
    disable_speaker();
}

extern "C" void play_song(Song *song) {
    play_song_impl(song);
}
