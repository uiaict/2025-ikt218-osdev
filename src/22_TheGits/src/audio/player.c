#include "audio/player.h"
#include "memory/memory.h"
#include "audio/speaker.h"
#include "libc/scrn.h"
#include "pit/pit.h"


SongPlayer* create_song_player(){
    // Allokerer minne til en ny SongPlayer-struktur
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    
    // Setter funksjonspekeren til å bruke vår implementasjon
    player->play_song = play_song_impl;
    
    return player;
}
void play_song_impl(Song *song){
    enable_speaker(); // Enable the speaker before playing the song
    for (uint32_t i = 0; i < song->length; i++) {
        Note* note = &song->notes[i]; // Get the current note
        // Display note details (frequency and duration)
       // printf("Note %d: freq = %d Hz, duration = %d ms\n", i, note->frequency, note->duration);
        play_sound(note->frequency); // Play the sound for the note
        sleep_interrupt(note->duration); // Delay for the duration of the note
        stop_sound(); // Stop the sound after playing the note
    }
    disable_speaker(); // Disable the speaker after playing the song

}

void play_song(Song *song){
    play_song_impl(song);
    // This function serves as a wrapper around play_song_impl, allowing for consistent usage of the SongPlayer interface.
}

void play_music(Note* notes, uint32_t length) {

    Song song = {notes, length}; // Create a song object with the provided notes and length
    SongPlayer* player = create_song_player(); // Create a new song player

    printf("Playing Song...\n");
    player->play_song(&song);
    printf("Finished playing the song.\n");

    free(player); // Free the allocated memory for the player
}