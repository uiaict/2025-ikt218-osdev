#include <libc/stdbool.h>
#include "song/song.h"
#include <pit.h>
#include <kernel/common.h>
#include <libc/stdio.h>
#include "io.h"
#include "kernel_memory.h"
#include "keyboard.h"

#ifdef __cplusplus
extern "C" {
#endif

void enable_speaker(){
    // Read the current state of the PC speaker control register
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    // Set bits 0 and 1 to enable the speaker
    if ((speaker_state & 0x03) != 0x03) {
        outb(PC_SPEAKER_PORT, speaker_state | 0x03);
    }
}

void disable_speaker() {
    // Turn off the PC speaker
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, speaker_state & 0xFC);
}

void stop_sound() {
    // Stop the sound by clearing bit 1 (speaker data), leaving bit 0 (speaker gate) unchanged
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, speaker_state & 0xFD); // Clear bit 1
}

void play_sound(uint32_t frequency) {
    if (frequency == 0) {
        return;
    }

    uint16_t divisor = (uint16_t)(PIT_BASE_FREQUENCY / frequency);
    outb(PIT_CMD_PORT, 0xB6); 
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor >> 8));
    enable_speaker();
}

bool play_song_impl(Song *song) {
    for (uint32_t i = 0; i < song->length; i++) {
        Note* note = &song->notes[i];
        play_sound(note->frequency);
        sleep_interrupt(note->duration);
        stop_sound();
        sleep_interrupt(10);

        // Check for 'n' kkey to interrupt the song
        if (keyboard_get_last_char() == 'n') {
            keyboard_clear_last_char();
            stop_sound();
            disable_speaker();
            return false;
        }
    }
    disable_speaker();
    return true;
}

bool play_song(Song *song) {
    return play_song_impl(song);
}

SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer)); // Use malloc since primarily C
    player->play_song = play_song;
    return player;
}

#ifdef __cplusplus
}
#endif