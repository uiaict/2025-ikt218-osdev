#include "pcspkr.h"
#include "timer.h"
#include <stddef.h>

#define PIT_CHANNEL2 0x42
#define PIT_COMMAND  0x43
#define SPEAKER_PORT 0x61

typedef struct {
    const char* name;
    u32int frequency;
} note_t;

static note_t notes[] = {
    {"C4",  262},
    {"C#4", 277},
    {"D4",  294},
    {"D#4", 311},
    {"E4",  330},
    {"F4",  349},
    {"F#4", 370},
    {"G4",  392},
    {"G#4", 415},
    {"A4",  440},
    {"A#4", 466},
    {"B4",  494},
    {"C5",  523},
    {NULL, 0}
};

void init_pcspkr() {
    pcspkr_stop();
}

static u32int note_to_frequency(const char* note) {
    if (!note || *note == '\0') {
        return 440;
    }
    
    for (int i = 0; notes[i].name != NULL; i++) {
        const char* a = notes[i].name;
        const char* b = note;
        u8int match = 1;
        
        while (*a && *b) {
            if (*a++ != *b++) {
                match = 0;
                break;
            }
        }
        
        if (match && *a == '\0' && *b == '\0') {
            return notes[i].frequency;
        }
    }
    
    return 440;
}

void pcspkr_play_tone(u32int frequency, u32int duration) {
    u32int divisor = 1193180 / frequency;
    
    outb(PIT_COMMAND, 0xB6);
    
    outb(PIT_CHANNEL2, (u8int)(divisor & 0xFF));
    outb(PIT_CHANNEL2, (u8int)((divisor >> 8) & 0xFF));
    
    u8int tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp | 3);
    
    sleep(duration);
    
    pcspkr_stop();
}

void pcspkr_play_note(const char* note, u32int duration) {
    u32int frequency = note_to_frequency(note);
    pcspkr_play_tone(frequency, duration);
}

void pcspkr_stop() {
    u8int tmp = inb(SPEAKER_PORT) & 0xFC;
    outb(SPEAKER_PORT, tmp);
}

void pcspkr_beep() {
    pcspkr_play_tone(1000, 100);
}