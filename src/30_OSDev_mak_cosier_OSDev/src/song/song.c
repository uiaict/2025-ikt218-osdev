#include "libc/song.h"

#include "libc/pit.h"
#include "libc/common.h"
#include "libc/stdio.h"
#include "libc/memory.h"  // for malloc

void speaker_enable() 
{
    uint8_t status = inb(PC_SPEAKER_PORT);

    // Enable speaker only if bits 0 and 1 are not set
    if (status != (status | 0x03)) 
    {
        outb(PC_SPEAKER_PORT, status | 0x03);
    }
}

void speaker_disable() 
{
    uint8_t status = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, status & 0xFC); // Clear bits 0 and 1
}

void sound_start(uint32_t hz) 
{
    if (hz == 0) return;

    uint16_t pit_divisor = (uint16_t)(PIT_BASE_FREQUENCY / hz);

    outb(PIT_CMD_PORT, 0b10110110);
    outb(PIT_CHANNEL2_PORT, (uint8_t)(pit_divisor & 0xFF));
    outb(PIT_CHANNEL2_PORT, (uint8_t)(pit_divisor >> 8));

    uint8_t status = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, status | 0x03);
}

void sound_stop() 
{
    uint8_t status = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, status & ~0x03);
}

void internal_play_song(Song *track) 
{
    speaker_enable();
    for (uint32_t idx = 0; idx < track->length; idx++) 
    {
        Note *current = &track->notes[idx];
        kprint("Note %d: Freq = %d Hz, Duration = %d ms\n", idx, current->frequency, current->duration);
        sound_start(current->frequency);
        sleep_interrupt(current->duration);
        sound_stop();
    }

    speaker_disable();
}

void play_song(Song *track) 
{
    internal_play_song(track);
}

SongPlayer* create_song_player()
{
    SongPlayer* player = (SongPlayer*)malloc(sizeof(SongPlayer));
    player->play_song = play_song;
    return player;
}
