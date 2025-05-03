/*  src/song.c  – plays a sequence of notes through the PC speaker  */
#include "song.h"
#include "pit.h"
#include "io.h"
#include "libc/stdio.h"

/* --------------------------------------------------  low-level helpers */

static inline void speaker_on(void)
{
    uint8_t v = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, v | 0x03);        /* set bits 0 & 1 */
}

static inline void speaker_off(void)
{
    uint8_t v = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, v & 0xFC);        /* clear bits 0 & 1 */
}

static void program_frequency(uint32_t hz)
{
    if (hz == 0) {          /* silence / rest */
        speaker_off();
        return;
    }

    uint32_t divisor = PIT_BASE_FREQUENCY / hz;
    if (divisor < 1)   divisor = 1;
    if (divisor > 0xFFFF) divisor = 0xFFFF;

    /* toggle gate LOW so the new value will latch */
    speaker_off();

    /* program PIT channel 2: mode 3, lobyte/hibyte */
    outb(PIT_CMD_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, divisor >> 8);

    speaker_on();
}

/* --------------------------------------------------  high-level player */

void play_song_impl(Song *song)
{
    if (!song || !song->notes || song->length == 0) {
        printf("Bad song struct\n");
        return;
    }

    printf("Starting song (%u notes)\n", (unsigned)song->length);

    for (uint32_t i = 0; i < song->length; ++i) {
        Note *n = &song->notes[i];

        printf("Note %u/%u – %u Hz for %u ms\n",
               i + 1, (unsigned)song->length,
               (unsigned)n->frequency,
               (unsigned)n->duration);

        program_frequency(n->frequency);
        sleep_interrupt(n->duration);

        /* short gap to make the next note audible */
        program_frequency(0);
        sleep_interrupt(25);
    }

    speaker_off();
    printf("Song finished\n");
}

/* singleton player object ------------------------------------------------ */
static SongPlayer player = { .play_song = play_song_impl };

SongPlayer *create_song_player(void) { return &player; }
