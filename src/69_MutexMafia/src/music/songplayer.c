//#include "libc/stdio.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/string.h"

#include "../music/songplayer.h"
#include "../pit/pit.h"
#include "../io/printf.h"
#include "../utils/utils.h"
#include "../memory/malloc.h"


Song songs[] = {

    {battlefield_1942_theme, sizeof(battlefield_1942_theme) / sizeof(Note)},
    {starwars_theme, sizeof(starwars_theme) / sizeof(Note)},
    {mario, sizeof(mario) / sizeof(Note)},
    {fader_jakob, sizeof(fader_jakob) / sizeof(Note)},
    {song_5, sizeof(song_5) / sizeof(Note)},
    {song_6, sizeof(song_6) / sizeof(Note)},
    {song_7, sizeof(song_7) / sizeof(Note)},
    {smash_bros_brawl_intro, sizeof(smash_bros_brawl_intro) / sizeof(Note)},
    //{song_8, sizeof(song_8) / sizeof(Note)},
    
    
    
};
uint32_t n_songs = sizeof(songs) / sizeof(Song);

const char* song_names[] = {
    "1. Battlefield 1942",
    "2. Star Wars",
    "3. Mario",
    "4. Fader Jakob",
    "5. Song 5",
    "6. Song 6",
    "7. Song 7",
    "8. Smash Bros Brawl",
    //"8. Song 8",
};



void enable_speaker() {

    uint8_t status = inPortB(PC_SPEAKER_PORT);          // leser status på PC speaker control port
    if ((status & 0x03) != (0x03)) {                        // sjekker om høytalleren allerede er aktivert
        status |= 0x03;                                   // setter bit 0 og bit 1 for å aktivere høyttaleren
         outPortB (PC_SPEAKER_PORT, status);            // setter ny status til PC speaker control port
    }
}

void disable_speaker() {
    uint8_t status = inPortB(PC_SPEAKER_PORT);          // leser status på PC speaker control port
    status &= ~0xFC;                                    // nullstiller bit 0 og bit 1 for å deaktivere høyttaleren
    outPortB (PC_SPEAKER_PORT, status);                 // nullstiller bit 0 og bit 1 for å deaktivere høyttaleren
}

void play_sound(uint32_t frequency) {

    if (frequency == 0) {
        mafiaPrint("No sound to play (frequency is 0).\n");
        return;
    }
    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;      // beregner divisor for å sette PIT frekvens
    outPortB(PIT_CMD_PORT, 0xB6);                           // setter PIT til binær telling, modus 3, og tilgangsmodus (lav/høy byte)
    outPortB(PIT_CHANNEL2_PORT, divisor & 0xFF);            // sender lav byte av divisor til PIT kanal 2
    outPortB(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);     // sender høy byte av divisor til PIT kanal 2
    enable_speaker(); 


}

void stop_sound() {

    uint8_t status = inPortB(PC_SPEAKER_PORT);
    status &= ~0x03;                                    // nullstiller bit 0 og bit 1 for å deaktivere høyttaleren
    outPortB(PC_SPEAKER_PORT, status);                 // Deaktiverer høyttaleren ved å nullstille bit 0 og bit 1
}

void play_song_impl(Song *song) {

    enable_speaker();
    for (size_t i = 0; i < song->note_count; i++) {
        Note note = song->notes[i];
        mafiaPrint("Playing note: Frequency: %d, Duration: %d ms\n", note.frequency, note.duration);
        play_sound(note.frequency);
        sleep_interrupt(note.duration);
        disable_speaker();
    }
}

void play_song(Song *song) {

    
    play_song_impl(song);
}


SongPlayer* create_song_player() {
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song_impl;
    return player;
}



void song_menu(){
    char input[2];
    SongPlayer* player = create_song_player();

    while(1){
        mafiaPrint("Select a song to play:\n");
        mafiaPrint("2. Battlefield 1942 Theme\n");
        mafiaPrint("1. Star Wars Theme\n");
        mafiaPrint("3. Mario Theme\n");
        mafiaPrint("4. Fader Jakob\n");
        mafiaPrint("5. Song 5\n");
        mafiaPrint("6. Song 6\n");
        mafiaPrint("7. Song 7\n");
        mafiaPrint("8. Smash Bros Brawl\n");
        mafiaPrint("9. Exit\n");

        get_input(input, sizeof(input));

        if (input[0] == '9' || input[0] == 'Q') {
            mafiaPrint("Exiting song menu.\n");
            break;
        }

        int choice = input[0] - '1';
        if (choice >= 0 && (uint32_t)choice < n_songs) {
            mafiaPrint("Playing %s...\n", song_names[choice]);
            player->play_song(&songs[choice]);
            mafiaPrint("Finished.\n");
            stop_sound();
        } else {
            mafiaPrint("Invalid choice.\n");
        }
    }
    free(player);

}