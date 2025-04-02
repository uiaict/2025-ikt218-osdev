#include "song/song.h"
#include "libc/pit.h"
#include "libc/io.h"
#include "libc/stdio.h"

#define PC_SPEAKER_PORT 0x61
#define PIT_CHANNEL2_PORT 0x42

void enable_speaker()
{
    uint8_t tmp = inb(PC_SPEAKER_PORT);
    if (tmp != (tmp | 3))
    {
        outb(PC_SPEAKER_PORT, tmp | 3); // Enable speaker gate and data
    }
}

void disable_speaker()
{
    uint8_t tmp = inb(PC_SPEAKER_PORT) & 0xFC; // Clear bits 0 and 1
    outb(PC_SPEAKER_PORT, tmp);
}

void play_sound(uint32_t frequency)
{
    if (frequency == 0)
        return;

    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;

    // Configure PIT for channel 2 (used by speaker)
    outb(PIT_CMD_PORT, 0xB6);                                  // 10110110 = binary, mode 3, lobyte/hibyte, channel 2
    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));        // Low byte
    outb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF)); // High byte

    enable_speaker();
}

void stop_sound()
{
    disable_speaker();
}

void play_song_impl(Song *song)
{
    terminal_write("Playing Song...\n");

    enable_speaker();
    for (uint32_t i = 0; i < song->length; i++)
    {
        Note note = song->notes[i];
        char buffer[16];

        // Print frequency
        terminal_write("Note: ");
        itoa(note.frequency, buffer, 10);
        terminal_write(buffer);
        terminal_write(" Hz, Duration: ");
        itoa(note.duration_ms, buffer, 10);
        terminal_write(buffer);
        terminal_write(" ms\n");

        play_sound(note.frequency);
        sleep_interrupt(note.duration_ms);
        stop_sound();
    }
    disable_speaker();

    terminal_write("Finished playing song\n");
}
