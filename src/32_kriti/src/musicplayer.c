#include "musicplayer.h"
#include "memory.h"
#include "kprint.h"
#include "pit.h"
#include "isr.h"

// Play a song using the PC speaker
void play_song_impl(Song* song) {
    if (!song || !song->notes || song->note_count == 0) {
        kprint("Invalid song or empty song\n");
        return;
    }
    
    kprint("Playing song with ");
    kprint_dec(song->note_count);
    kprint(" notes\n");
    
    for (size_t i = 0; i < song->note_count; i++) {
        Note* note = &song->notes[i];
        
        kprint("Note ");
        kprint_dec(i);
        kprint(": ");
        kprint_dec(note->freq);
        kprint(" Hz for ");
        kprint_dec(note->duration);
        kprint(" ms\n");
        
        if (note->freq > 0) {
            // Use the beep_blocking function from pit.c to play the note
            beep_blocking(note->freq, note->duration);
        } else {
            // This is a rest (silence) - just wait for the duration
            sleep_interrupt(note->duration);
        }
        
        // Small pause between notes for clarity
        sleep_interrupt(30);
    }
    
    kprint("Song complete\n");
}

// Create a new SongPlayer
SongPlayer* create_song_player(void) {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    if (player) {
        player->play_song = play_song_impl;
    } else {
        kprint("Failed to allocate memory for SongPlayer\n");
    }
    return player;
}