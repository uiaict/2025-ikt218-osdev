#include "song.h"
#include "pcspeaker.h"
#include "kernel/pit.h"   
#include "printf.h"  
#include "kernel/memory.h"   

extern void sleep_busy(uint32_t milliseconds);


void play_song_impl(Song *song) {
    if (!song || !song->notes || song->note_count == 0) {
        printf("Error: No song\n");
        return;
    }

    for (size_t i = 0; i < song->note_count; i++) {
        Note current_note = song->notes[i];

        if (current_note.frequency > 0) {
            play_sound(current_note.frequency);
        } else {
            stop_sound(); // Ensure speaker is off during rests
        }

        if (current_note.duration > 0) {
            sleep_busy(current_note.duration);
        }

        stop_sound();
    }

    disable_speaker(); //  Speaker is off after song
}

SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    if (!player) {
        printf("Error: Cant allocate memory for song\n");
        return NULL; // Or panic
    }
    player->play_song = play_song_impl;
    return player;
}


void play_music() {
    Song songs[] = {
        // {music_topgun, sizeof(music_topgun) / sizeof(Note)}
        // {music_smoke, sizeof(music_smoke) / sizeof(Note)}
        // {music_richmans, sizeof(music_richmans) / sizeof(Note)}
        {music_nokia, sizeof(music_nokia) / sizeof(Note)}
    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);

    SongPlayer* player = create_song_player();
    if (!player) {
        printf("Cannot play music\n");
        return; // Exit if player creation failed
    }

        for(uint32_t i = 0; i < n_songs; i++) {
            player->play_song(&songs[i]);
            sleep_busy(1000); // Pause for 1 second
        }
     free(player); // Free the player after use, but wont ever reach here due to inf. loop
}