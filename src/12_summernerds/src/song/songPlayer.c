#include <song/song.h>
#include "kernel/pit.h"
#include <libc/stdio.h>
#include "common.h"

void enable_speaker()
{
    uint8_t speaker_state = inb(0x61);
    outb(0x61, speaker_state | 3);
}

void disable_speaker()
{
    uint8_t speaker_state = inb(0x61);
    outb(0x61, speaker_state & 0xFC);
}

void play_sound(uint32_t frequency)
{
    if (frequency == 0)
        return;

    uint32_t Div = 1193180 / frequency;
    outb(0x43, 0xB6); // PIT command port: binary, mode 3, channel 2

    outb(0x42, (uint8_t)(Div & 0xFF));        // Low byte
    outb(0x42, (uint8_t)((Div >> 8) & 0xFF)); // High byte

    uint8_t tmp = inb(0x61);
    if ((tmp & 3) != 3)
    {
        outb(0x61, tmp | 3);
    }
}

// make it shut up
static void nosound()
{
    uint8_t tmp = inb(0x61) & 0xFC;

    outb(0x61, tmp);
}

void beep()
{
    play_sound(1000);
    // timer_wait(10);
    nosound();
}

void stop_sound()
{
    uint8_t speaker_state = inb(0x61);
    outb(0x61, speaker_state & ~3);
}

void play_song_impl(Song *song)
{
    enable_speaker();

    for (uint32_t i = 0; i < song->length; i++)
    {
        Note *note = &song->notes[i];
        printf("Playing note with frequency %d in length %d.\n", note->frequency, note->duration);
        play_sound(note->frequency);
        sleep_interrupt(note->duration);
        stop_sound();
    }

    disable_speaker();
}

void play_song(Song *song)
{
    play_song_impl(song);
}