#include "song/song.h"
#include "port_io.h"
#include "pit.h"

void play_song_impl(Song* song) {
    for (size_t i = 0; i < song->note_count; ++i) {
        Note note = song->notes[i];

        // OPTIONAL DEBUG
        // terminal_write("play_sound: ");
        // terminal_putint(note.frequency);
        // terminal_putchar('\n');

        play_sound(note.frequency);
        sleep_busy(note.duration);
        stop_sound();
        sleep_busy(10); // short gap
    }
}
