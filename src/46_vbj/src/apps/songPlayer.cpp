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
    uint8_t port_value = inb(PC_SPEAKER_PORT);  // Les inn nåværende status fra PC-høyttalerens port
    
    // Fjern bare bit 1 som styrer data for å stoppe lyden
    outb(PC_SPEAKER_PORT, port_value & ~0b10);  // Fjerner bit 1, men lar bit 0 (høyttalerens "gate") være intakt
}

void play_song_impl(Song *song) {

    enable_speaker();

    for (size_t i = 0; i < song -> length; i++)
        {
            Note* note = &song->notes[i];
            printf("Playing note: %d Hz for %d ms\n", note->frequency, note->duration);

            play_sound(note->frequency);

            sleep_interrupt(note->duration); // Bruk PIT til å vente på riktig tid

            stop_sound();
            //sleep_interrupt(20); // Legg til en liten pause mellom noter
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