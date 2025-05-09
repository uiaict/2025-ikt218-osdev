#include <libc/stdbool.h>
#include "song.h"
#include <pit.h>
#include <libc/stdio.h>
#include "io.h"
#include "kernel_memory.h"
#include "keyboard.h"

void enable_speaker()
{
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    if ((speaker_state & 0x03) != 0x03)
    {
        outb(PC_SPEAKER_PORT, speaker_state | 0x03);
    }
}

void disable_speaker()
{
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, speaker_state & 0xFC);
}

void stop_sound()
{
    uint8_t speaker_state = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, speaker_state & 0xFD);
}

void play_sound(uint32_t frequency)
{
    if (frequency == 0)
    {
        return;
    }

    uint16_t divisor = (uint16_t)(PIT_BASE_FREQUENCY / frequency);
    outb(PIT_CMD_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor >> 8));
    enable_speaker();
}

SongResult play_song_impl(SongPlayer *player, Song *song)
{
    if (!player || !song)
        return SONG_COMPLETED;

    player->is_playing = true;

    for (uint32_t i = 0; i < song->length; i++)
    {
        Note *note = &song->notes[i];
        play_sound(note->frequency);
        sleep_interrupt(note->duration);
        stop_sound();

        char current_key = keyboard_get_last_char();
        if (current_key != 0)
        {
            keyboard_clear_last_char();
            stop_sound();
            disable_speaker();
            player->is_playing = false;

            if (current_key == 'd')
                return SONG_INTERRUPTED_NEXT;
            if (current_key == '\b')
                return SONG_INTERRUPTED_BACK;
            if (current_key == 's')
                return SONG_INTERRUPTED_SELECT;
            if (current_key == 'a')
                return SONG_INTERRUPTED_PREV;
        }
    }

    disable_speaker();
    player->is_playing = false;
    return SONG_COMPLETED;
}

SongResult play_song(SongPlayer *player, Song *song)
{
    return play_song_impl(player, song);
}

SongPlayer *create_song_player()
{
    SongPlayer *player = (SongPlayer *)malloc(sizeof(SongPlayer));
    if (player)
    {
        player->play_song = play_song;
        player->is_playing = false;
    }
    return player;
}

void free_song_player(SongPlayer *player)
{
    if (player)
    {
        stop_sound();
        disable_speaker();
        free(player);
    }
}
