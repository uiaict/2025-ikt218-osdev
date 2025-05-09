#include "song.h"
#include "pit.h"
#include "ports.h"
#include "libc/stdio.h"
#include "terminal.h"
#include "libc/string.h"
#include "libc/stdint.h"

void enable_speaker() {
    // Read the current state of the PC speaker control register
    uint8_t speaker_state = inb(PC_SPEAKER);
    /*
    Bit 0: Speaker gate
            0: Speaker disabled
            1: Speaker enabled
    Bit 1: Speaker data
            0: Data is not passed to the speaker
            1: Data is passed to the speaker
    */
    // Check if bits 0 and 1 are not set (0 means that the speaker is disabled)
    if (speaker_state != (speaker_state | 3)) {
        // If bits 0 and 1 are not set, enable the speaker by setting bits 0 and 1 to 1
        outb(PC_SPEAKER, speaker_state | 3);
    }
}

void disable_speaker() {
    // Turn off the PC speaker
    uint8_t speaker_state = inb(PC_SPEAKER);
    outb(PC_SPEAKER, speaker_state & 0xFC);
}

void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        return;
    }

    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);

    outb(PIT_COMMAND, 0xb6); 
    outb(PIT_CHANNEL2, (uint8_t)(divisor));
    outb(PIT_CHANNEL2, (uint8_t)(divisor >> 8));

    uint8_t tmp = inb(PC_SPEAKER);
    if (tmp != (tmp | 3)) {
        outb(PC_SPEAKER, tmp | 3);
    }
}

void stop_sound(){
    uint8_t tmp = inb(PC_SPEAKER) & 0xFC;
    outb(PC_SPEAKER, tmp);
}

void play_song_impl(Song *song) {
    enable_speaker();
    for (uint32_t i = 0; i < song->length; i++) {
        Note* note = &song->notes[i];
        play_sound(note->frequency);
        sleep_interrupt(note->duration);
        stop_sound();
    }
    disable_speaker();
}

void play_song(Song *song) {
    play_song_impl(song);
}