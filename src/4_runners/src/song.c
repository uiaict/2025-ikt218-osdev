#include "song.h"
#include "pit.h"
#include "io.h"
#include "libc/stdio.h"

/* -------------------------------------------------- low-level helpers */

void speaker_control(bool on) {
    uint8_t v = inb(PC_SPEAKER_PORT);
    if (on) {
        outb(PC_SPEAKER_PORT, v | 0x03); // Set bits 0 & 1
    } else {
        outb(PC_SPEAKER_PORT, v & 0xFC); // Clear bits 0 & 1
    }
}

void set_frequency(uint32_t hz) {
    if (hz == 0) { // Silence / rest
        speaker_control(false);
        return;
    }

    uint32_t divisor = PIT_BASE_FREQUENCY / hz;
    if (divisor < 1) divisor = 1;
    if (divisor > 0xFFFF) divisor = 0xFFFF;

    // Toggle gate LOW so the new value will latch
    speaker_control(false);

    // Program PIT channel 2: mode 3, lobyte/hibyte
    outb(PIT_CMD_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, divisor >> 8);

    speaker_control(true);
}

void delay_ms(uint32_t ms) {
    // Approximate delay: adjust multiplier based on CPU speed
    // 5000 iterations ~ 1ms on typical QEMU setup
    for (uint32_t i = 0; i < ms * 20000; i++) {
        asm volatile("nop");
    }
}

/* -------------------------------------------------- high-level player */

static void play_song_impl(Song *song) {
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
            speaker_control(false);
            printf("Rest for %ums\n", (unsigned)n->duration);
        } else {
            set_frequency(n->frequency);
            printf("Playing note %u/%u: %uHz for %ums\n", 
                   i + 1, (unsigned)song->length, 
                   (unsigned)n->frequency, 
                   (unsigned)n->duration);
        }

        delay_ms(n->duration);
        
        // Brief silence between notes
        speaker_control(false);
        delay_ms(20);  // Short gap
    }

    // Restore original speaker state when done
    outb(PC_SPEAKER_PORT, original_speaker_state);
    printf("Song finished\n");
}

/* singleton player object */
static SongPlayer player = { .play_song = play_song_impl };

SongPlayer* create_song_player(void) {
    return &player;
}