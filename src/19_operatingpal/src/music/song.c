#include "music/song.h"
#include "music/sound.h"
#include "music/notes.h"
#include "interrupts/pit.h"
#include "libc/stdio.h"
#include "libc/stdbool.h"

bool is_song_playing = false;
static Song* current_song = NULL;
static uint32_t current_note = 0;
static uint32_t note_elapsed = 0;
static uint32_t current_freq = 0; // Track last played frequency

void play_song(Song* song) {
    current_song = song;
    current_note = 0;
    note_elapsed = 0;
    current_freq = 0;
    is_song_playing = true;
}

void stop_song() {
    is_song_playing = false;
    stop_sound();
    current_freq = 0;
}

void update_song_tick() {
    if (!is_song_playing || current_song == NULL) return;

    note_elapsed++;

    Note note = current_song->notes[current_note];
    if (note_elapsed >= note.duration) {
        current_note++;
        note_elapsed = 0;

        if (current_note >= current_song->note_count) {
            current_note = 0; // loop song
        }

        note = current_song->notes[current_note];
        if (note.frequency == 0) {
            stop_sound();
            current_freq = 0;
        } else {
            if (note.frequency != current_freq) {
                play_sound(note.frequency);
                current_freq = note.frequency;
            }
        }
    }
}
