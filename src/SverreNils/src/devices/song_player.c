#include <stdint.h>
#include "pit.h"
#include "song/song.h"
#include "libc/stdio.h"


#define PC_SPEAKER_PORT 0x61
#define PIT_CHANNEL2    0x42
#define PIT_COMMAND     0x43


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
    outb(PC_SPEAKER_PORT, tmp | 3); 
}


void disable_speaker() {
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, tmp & 0xFC); 
}


void play_sound(uint32_t freq) {
    if (freq == 0) return;

    uint32_t divisor = 1193182 / freq;

    outb(PIT_COMMAND, 0xB6); 
    outb(PIT_CHANNEL2, (uint8_t)(divisor & 0xFF));       
    outb(PIT_CHANNEL2, (uint8_t)((divisor >> 8) & 0xFF)); 

    enable_speaker();
}


void stop_sound() {
    disable_speaker();
}


void play_song_impl(Song* song) {
    for (size_t i = 0; i < song->note_count; i++) {
        Note note = song->notes[i];
        printf("Note: %d Hz, duration: %d ms\n", (int)note.frequency, (int)note.duration_ms);

        if (note.frequency > 0) {
            play_sound(note.frequency);
        } else {
            stop_sound(); 
        }

        sleep_interrupt(note.duration_ms);

        stop_sound(); 
    }


    stop_sound();
}
