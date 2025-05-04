#include "music/song.h"
#include "music/sound.h"       // Hent speaker-funksjoner herfra
#include "music/notes.h"      // For å bruke Note og Song
#include "interrupts/pit.h"    // For sleep_interrupt()
#include "libc/stdio.h"

void play_song(Song* song) {
    printf("Playing State Anthem of the Soviet Union \n", (int)song->note_count);

    uint32_t prev_freq = 0;

    for (size_t i = 0; i < song->note_count; i++) {
        Note note = song->notes[i];

        if (note.frequency == 0) {
            stop_sound(); // Pause
        } else {
            if (note.frequency != prev_freq) {
                play_sound(note.frequency); // Bare sett ny frekvens hvis den endrer seg
                prev_freq = note.frequency;
            }
        }

        sleepInterrupt(note.duration);
    }

    stop_sound(); // Til slutt
    printf("Song done.\n");
}

