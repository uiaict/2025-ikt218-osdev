#include "musicPlayer.h"


void enable_speaker() {
    uint8_t tmp;
    // Disable interrupts before configuring the PIT for the speaker
    asm volatile("cli;");
    // Enable the speaker (PC speaker control)
    tmp = inPortB(0x61);
    if (tmp != (tmp | 3)) {
        outPortB(0x61, tmp | 3);
    }

    // Re-enable interrupts after speaker setup
    asm volatile("sti;");
}

void disable_speaker() {
    uint8_t tmp = inPortB(0x61) & 0xFC;
 	outPortB(0x61, tmp);
}

void play_sound(uint32_t frequency) {

    if(frequency == 0) {
        return;
    }
    asm volatile("cli;");
    uint32_t divisor = PIT_BASE_FREQUENCY/frequency;
    outPortB(0x43, 0xB6); // Set PIT for Channel 2, mode 3
    outPortB(0x42, (uint8_t) (divisor));       // Low byte
    outPortB(0x42, (uint8_t) (divisor >> 8));  // High byte
    enable_speaker();
    asm volatile("sti;");
}

void stop_sound() {
    uint8_t tmp = inPortB(0x61) & ~0x01;
    outPortB(0x61, tmp); 
}

void play_song_impl(Song *song) {
    enable_speaker();
    for(uint32_t i = 0; i < song->length; i++) {
        uint32_t noteDuration = song->notes[i].duration;
        uint32_t noteFrequency = song->notes[i].frequency;
        
        play_sound(noteFrequency);
        //printf("[Duration: %d] && ",noteDuration);
        //printf("[Frequency: %d]\n",noteFrequency);
        sleepInterrupt(noteDuration);
        stop_sound();
    }
    disable_speaker();
}

void play_song(Song *song) {
    play_song_impl(song);
}