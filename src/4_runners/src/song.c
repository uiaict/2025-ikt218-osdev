/* src/song.c – plays a sequence of notes through the PC speaker */
#include "song.h"
#include "pit.h"
#include "io.h"
#include "libc/stdio.h"

/* -------------------------------------------------- low-level helpers */

static inline void speaker_on(void)
{
    uint8_t v = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, v | 0x03); /* set bits 0 & 1 */
}

static inline void speaker_off(void)
{
    uint8_t v = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, v & 0xFC); /* clear bits 0 & 1 */
}

static void program_frequency(uint32_t hz)
{
    if (hz == 0) { /* silence / rest */
        speaker_off();
        return;
    }

    uint32_t divisor = PIT_BASE_FREQUENCY / hz;
    if (divisor < 1) divisor = 1;
    if (divisor > 0xFFFF) divisor = 0xFFFF;

    /* toggle gate LOW so the new value will latch */
    speaker_off();

    /* program PIT channel 2: mode 3, lobyte/hibyte */
    outb(PIT_CMD_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, divisor >> 8);

    speaker_on();
}

/* Delay in milliseconds using a busy-wait loop */
 void delay_ms(uint32_t ms)
{
    // Approximate delay: 500 iterations ~ 1ms on typical QEMU setup
    for (uint32_t i = 0; i < ms * 50000; i++)
    {
        asm volatile("nop");
    }
}

/* -------------------------------------------------- Background music state */

static struct {
    Song *current_song;
    uint32_t current_note;
    uint32_t time_accumulator; // in milliseconds
    bool is_playing;
    bool loop;
} background_music = {0};

/* Start background music playback */
void start_background_music(Song *song, bool loop)
{
    background_music.current_song = song;
    background_music.current_note = 0;
    background_music.time_accumulator = 0;
    background_music.is_playing = true;
    background_music.loop = loop;

    if (song && song->notes && song->length > 0) {
        Note *n = &song->notes[0];
        program_frequency(n->frequency);
    }
}

/* Stop background music playback */
void stop_background_music(void)
{
    background_music.is_playing = false;
    background_music.current_song = NULL;
    background_music.current_note = 0;
    background_music.time_accumulator = 0;
    speaker_off();
}

/* Update background music on each tick */
void song_tick(uint32_t tick_ms)
{
    if (!background_music.is_playing || !background_music.current_song || 
        background_music.current_note >= background_music.current_song->length) {
        if (background_music.loop && background_music.current_song) {
            background_music.current_note = 0; // Loop back to start
            background_music.time_accumulator = 0;
            Note *n = &background_music.current_song->notes[0];
            program_frequency(n->frequency);
        } else {
            stop_background_music();
        }
        return;
    }

    Note *n = &background_music.current_song->notes[background_music.current_note];
    background_music.time_accumulator += tick_ms;

    // Check if the current note's duration (including gap) has elapsed
    uint32_t total_duration = n->duration + 5; // 5ms gap between notes
    if (background_music.time_accumulator >= total_duration) {
        background_music.current_note++;
        background_music.time_accumulator = 0;

        if (background_music.current_note < background_music.current_song->length) {
            Note *next_note = &background_music.current_song->notes[background_music.current_note];
            program_frequency(next_note->frequency);
        }
    } else if (background_music.time_accumulator >= n->duration) {
        // Silence gap between notes
        speaker_off();
    }
}

/* -------------------------------------------------- high-level player */

void play_song_impl(Song *song) {
    if (!song || !song->notes || song->length == 0) {
        printf("Bad song struct\n");
        return;
    }

    printf("Starting song (%u notes)\n", (unsigned)song->length);

    // Save the original speaker state
    uint8_t original_speaker_state = inb(PC_SPEAKER_PORT);

    for (uint32_t i = 0; i < song->length; ++i) {
        Note *n = &song->notes[i];
        
        // Skip notes with 0 frequency (consider them as rests)
        if (n->frequency == 0) {
            speaker_off();  // Use the helper function
            printf("Rest for %ums\n", (unsigned)n->duration);
        } else {
            // Use the helper function to program frequency
            program_frequency(n->frequency);
            
            printf("Playing note %u/%u: %uHz for %ums\n", 
                   i + 1, (unsigned)song->length, 
                   (unsigned)n->frequency, 
                   (unsigned)n->duration);
        }

        // Wait for the duration
        delay_ms(n->duration);
        
        // Brief silence between notes
        speaker_off();  // Use the helper function
        delay_ms(5);  // Reduced gap for smoother transition
    }

    // Restore original speaker state when done
    outb(PC_SPEAKER_PORT, original_speaker_state);
    printf("Song finished\n");
}

/* singleton player object ------------------------------------------------ */
static SongPlayer player = { .play_song = play_song_impl };

SongPlayer *create_song_player(void) { return &player; }

/* Wrapper for speaker control */
void speaker_control(bool enable) {
    if (enable) {
        speaker_on();
    } else {
        speaker_off();
    }
}