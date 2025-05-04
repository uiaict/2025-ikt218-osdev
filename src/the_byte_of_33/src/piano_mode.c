#include "piano_mode.h"
#include "io.h"
#include "kernel/boot_art.h"
#include <libc/stdint.h>
#include <libc/stdbool.h>

extern void play_sound(uint32_t frequency);
extern void stop_sound(void);
extern void enable_speaker(void);
extern void disable_speaker(void);
extern void sleep_busy(uint32_t milliseconds);

// Poll PS/2 status port until a scancode arrives, then return it
static uint8_t wait_scancode(void) {
    while (!(inb(0x64) & 1)) {
        __asm__ volatile("hlt");
    }
    return inb(0x60);
}

// Ignore break codes (>=0x80), return only make codes
static uint8_t wait_make(void) {
    uint8_t sc;
    do {
        sc = wait_scancode();
    } while (sc & 0x80);
    return sc;
}

// Piano stub, quits on Q
void piano_mode(void) {
    // Mask only keyboard IRQ
    uint8_t pic1_mask = inb(0x21);
    outb(0x21, pic1_mask | 0x02);  // mask IRQ1

    clear_screen();
    set_color(0x0E);
    puts("Piano Mode (press Q to go back):\n");
    puts("  [A S D F G H J K] play notes\n");
    puts("  [Q] Back\n");

    // ─── Draw the ASCII piano below the menu ────────────────────────────
    static const char* keyboard_art[] = {
        " ________________________________",
        "|  | | | |  |  | | | | | |  |   |",
        "|  | | | |  |  | | | | | |  |   |",
        "|  | | | |  |  | | | | | |  |   |",
        "|  |_| |_|  |  |_| |_| |_|  |   |",
        "|   |   |   |   |   |   |   |   |",
        "| C | D | E | F | G | A | B | C |",
        "|___|___|___|___|___|___|___|___|",
        "  a   s   d   f   g   h   j   k",
    };
    const int art_rows = sizeof(keyboard_art) / sizeof(*keyboard_art);
    for (int i = 0; i < art_rows; i++) {
        __draw_text(4 + i, keyboard_art[i]);
    }

    uint32_t last_played_freq = 0;
    uint8_t sc;

    while (1) {
        if (inb(0x64) & 1) {
            sc = inb(0x60);
            bool released = sc & 0x80;
            uint8_t code = sc & 0x7F;

            if (released) {
                if (last_played_freq != 0) {
                    stop_sound();
                    disable_speaker();
                    last_played_freq = 0;
                }
                continue;
            }

            if (code == 0x10) { // 'Q' pressed
                break;
            }

            switch (code) {
                case 0x1E:  // A
                    if (last_played_freq != 261) {
                        stop_sound();
                        play_sound(261);
                        last_played_freq = 261;
                    }
                    break;
                case 0x1F:  // S
                    if (last_played_freq != 293) {
                        stop_sound();
                        play_sound(293);
                        last_played_freq = 293;
                    }
                    break;
                case 0x20:  // D
                    if (last_played_freq != 329) {
                        stop_sound();
                        play_sound(329);
                        last_played_freq = 329;
                    }
                    break;
                case 0x21:  // F
                    if (last_played_freq != 349) {
                        stop_sound();
                        play_sound(349);
                        last_played_freq = 349;
                    }
                    break;
                case 0x22:  // G
                    if (last_played_freq != 392) {
                        stop_sound();
                        play_sound(392);
                        last_played_freq = 392;
                    }
                    break;
                case 0x23:  // H
                    if (last_played_freq != 440) {
                        stop_sound();
                        play_sound(440);
                        last_played_freq = 440;
                    }
                    break;
                case 0x24:  // J
                    if (last_played_freq != 493) {
                        stop_sound();
                        play_sound(493);
                        last_played_freq = 493;
                    }
                    break;
                case 0x25:  // K — high C (one octave above A)
                    if (last_played_freq != 523) {
                        stop_sound();
                        play_sound(523);
                        last_played_freq = 523;
                    }
                    break;
                default:
                    if (last_played_freq != 0) {
                        stop_sound();
                        last_played_freq = 0;
                    }
                    break;
            }
        }
    }

    stop_sound();
    clear_screen();
    outb(0x21, pic1_mask);
}
