/* ---------------------------------------------------------------------
    * Inspired by the pseduocode from the assignment 5
    ---------------------------------------------------------------------
*/

extern "C"
{
    #include "apps/song.h"
    #include "pit.h"
    #include "global.h"
    #include "terminal.h"

}


void enable_speaker() {
    uint8_t port_value = inb(PC_SPEAKER_PORT);
    if ((port_value & 3) != 3) {
        outb(PC_SPEAKER_PORT, port_value | 3);
    }
}

void disable_speaker() {
    uint8_t port_value = inb(PC_SPEAKER_PORT);
    if ((port_value & 3) != 0) {
        outb(PC_SPEAKER_PORT, port_value & ~0b11);
    }
}

void play_sound(uint32_t frequency) {
    if (frequency == 0)
    {
        return;
    }

    uint16_t divisor = PIT_BASE_FREQUENCY / frequency;

    outb(PIT_CMD_PORT, 0xb6);
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);

    enable_speaker();

}

void stop_sound() {
    uint8_t port_value = inb(PC_SPEAKER_PORT); 

    outb(PC_SPEAKER_PORT, port_value & ~0b10);  
}

void play_song_impl(Song *song) {

    enable_speaker();

    for (size_t i = 0; i < song -> length; i++)
        {
            Note* note = &song->notes[i];
            printf("Playing note: %d Hz for %d ms\n", note->frequency, note->duration);

            play_sound(note->frequency);

            sleep_interrupt(note->duration);

            stop_sound();
            //sleep_interrupt(20); 
        }
    printf("Songplayer finished playing\n");
    

    disable_speaker();
    
}

void play_song(Song *song) {
    play_song_impl(song);
}

SongPlayer* create_song_player() {
    auto* player = new SongPlayer();
    player->play_song = play_song_impl;
    return player;
}