#include "song_player.h"
#include "sound.h"
#include "pit.h"
#include "libc/stdio.h"
#include "libc/memory.h"
//#include "song/music_1.h"
#include "song/music.h"
#include "song/mario.h"
#include "song/tetris.h"
#include "song/zelda.h"

void play_song_impl(Song *song) {
    enable_speaker();
    for (size_t i = 0; i < song->note_count; i++) {
        Note note = song->notes[i];
        //speaker_beep(note.frequency, note.duration_ms);
        play_sound(note.frequency);
        //sleep_interrupt(note.duration_ms);
        sleep_busy(note.duration_ms);
        disable_speaker();
        printf("Playing note: %u Hz for %u ms\n", note.frequency, note.duration_ms);
        //sleep_busy(10);
    }
}

SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song_impl;
    return player;
}


void play_music() {
    Song songs[] = {
        //{music_1, MUSIC_1_LENGTH},
        {music_1, music_1_len},
        {music_6, music_6_len},
        //{mario_new, mario_new_len},
        //{music_mario, music_mario_len},
        //{music_tetris, music_tetris_len},
        //{music_zelda, music_zelda_len}

    };
    uint32_t n_songs = sizeof(songs) / sizeof(Song);

    SongPlayer* player = create_song_player();

    while (1) {
        for (uint32_t i = 0; i < n_songs; i++) {
            printf("Playing Song...\n");
            player->play_song(&songs[i]);
            printf("Finished playing the song.\n");
            sleep_busy(2000); // pause between songs
            reset_ticker();
        }
    }
}
