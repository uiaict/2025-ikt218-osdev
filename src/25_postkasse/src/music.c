#include "libc/stdint.h"
#include "libc/io.h"
#include "libc/music.h"
#include "libc/monitor.h"
#include "libc/pit.h"
#include "libc/song.h"
#include "libc/frequencies.h"

#define SPEAKER_PORT 0x61 //PC speaker port
#define PIT_CHANNEL2 0x42 //PIT channel 2 for audio
#define PIT_COMMAND 0x43 //PIT command port
#define MODE_3_SQUARE 0xB6 //Word to set PIT shannel 2 to mode 3

extern Note music_1[];
extern const size_t MUSIC_1_LENGTH;

//Sets bits 0 and 1 of the speaker port to route sound to the speaker
void enable_speaker() {
    uint8_t tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp | 3);
}

//Clears bits 0 and 1 to silence the speaker
void disable_speaker() {
    uint8_t tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp & 0xFC);
}

//Makes the speaker play a tone at the given freq
void play_sound(uint32_t frequency) {
    if (frequency == 0) return; //Dont play anything if the freq is 0
    uint16_t divisor = PIT_BASE_FREQUENCY / frequency; //Set the divisor
    outb(PIT_COMMAND, MODE_3_SQUARE); // Write the PIT control word
    outb(PIT_CHANNEL2, (uint8_t)(divisor & 0xFF));// Low byte
    outb(PIT_CHANNEL2, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    //Enable the speaker to route PIT channel 2 to it
    enable_speaker();
}

// Clear speaker bits
void stop_sound() {
    uint8_t tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp & 0xFC); 
}

//Loop thorugh each note and play them at the duration specified
void play_song(Note* notes, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        
        //Check if the freq is rest
        if (notes[i].frequency == R) {
            stop_sound();
        } else {
            play_sound(notes[i].frequency);
            monitor_write("Freq: ");
            monitor_write_dec(notes[i].frequency);
            monitor_write(". Duration: ");
            monitor_write_dec(notes[i].duration);
            monitor_newline();
        }

        //Sets the CPU to sleep to not get any other interrupots
        sleep_interrupt(notes[i].duration); 
    }
    stop_sound(); 
}