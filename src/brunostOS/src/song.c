#include "song/song.h"
#include "speaker.h"
#include "timer.h"


void play_song(struct song *song){

    enable_speaker();

    for (size_t i = 0; i < song->length; i++){
        
        struct note note = song->notes[i];

        play_sound(note.frequency);
        interrupt_sleep(note.duration);
        stop_sound();

    }
    
    disable_speaker();
}


struct song_player* create_song_player(){

    struct song_player* player;
    player->play_song = play_song;
    return player;
}