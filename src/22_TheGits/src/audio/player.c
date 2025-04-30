#include "audio/player.h"
#include "memory/memory.h"
#include "audio/speaker.h"
#include "libc/scrn.h"
#include "pit/pit.h"


SongPlayer* create_song_player(){
    // Allocats memory for the SongPlayer struct
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    
    player->play_song = play_song_impl;
    
    return player;
}
void play_song_impl(Song *song){
    enable_speaker(); 
    for (uint32_t i = 0; i < song->length; i++) {
        Note* note = &song->notes[i]; 
       
        play_sound(note->frequency);
        sleep_busy(5);
        sleep_interrupt(note->duration - 5);

        stop_sound(); 
    }
    disable_speaker();
}

void play_song(Song *song){
    play_song_impl(song);
}

void play_music(Note* notes, uint32_t length) {

    Song song = {notes, length}; // Create a song object with the provided notes and length
    SongPlayer* player = create_song_player(); 

    player->play_song(&song);

    stop_sound(); 
    disable_speaker(); 

    free(player); 
    player = NULL;
}

