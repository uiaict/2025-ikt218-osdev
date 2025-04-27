//#include "libc/stdio.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/string.h"

#include "../music/songplayer.h"
#include "../pit/pit.h"
#include "../io/printf.h"
#include "../utils/utils.h"





void enable_speaker() {

    uint8_t status = inPortB(PC_SPEAKER_PORT);          // leser status på PC speaker control port
    if ((status & 0x03) != (0x03)) {                        // sjekker om høytalleren allerede er aktivert
        status |= 0x03;                                   // setter bit 0 og bit 1 for å aktivere høyttaleren
         outPortB (PC_SPEAKER_PORT, status);            // setter ny status til PC speaker control port
    }
}

void disable_speaker() {
    uint8_t status = inPortB(PC_SPEAKER_PORT);          // leser status på PC speaker control port
    status &= ~0xFC;                                    // nullstiller bit 0 og bit 1 for å deaktivere høyttaleren
    outPortB (PC_SPEAKER_PORT, status);                 // nullstiller bit 0 og bit 1 for å deaktivere høyttaleren
}

void play_sound(uint32_t frequency) {

    if (frequency == 0) {
        mafiaPrint("No sound to play (frequency is 0).\n");
        return;
    }
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;      // beregner divisor for å sette PIT frekvens
    outPortB(PIT_CMD_PORT, 0xB6);                           // setter PIT til binær telling, modus 3, og tilgangsmodus (lav/høy byte)
    outPortB(PIT_CHANNEL2_PORT, divisor & 0xFF);            // sender lav byte av divisor til PIT kanal 2
    outPortB(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);     // sender høy byte av divisor til PIT kanal 2
    enable_speaker(); 


}

void stop_sound() {

    uint8_t status = inPortB(PC_SPEAKER_PORT);
    status &= ~0x03;                                    // nullstiller bit 0 og bit 1 for å deaktivere høyttaleren
    outPortB(PC_SPEAKER_PORT, status);                 // Deaktiverer høyttaleren ved å nullstille bit 0 og bit 1
}

void play_song_impl(Song *song) {

    enable_speaker();
    for (size_t i = 0; i < song->note_count; i++) {
        Note note = song->notes[i];
        mafiaPrint("Playing note: Frequency: %d, Duration: %d ms\n", note.frequency, note.duration);
        play_sound(note.frequency);
        sleep_interrupt(note.duration);
        disable_speaker();
    }
}

void play_song(Song *song) {
    play_song_impl(song);
}

