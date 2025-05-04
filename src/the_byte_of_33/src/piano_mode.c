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
    // Mask only keyboard IRQ, like above
    uint8_t pic1_mask = inb(0x21);
    outb(0x21, pic1_mask | 0x02);  // mask IRQ1

    __clear_screen();
    set_color(0x0E);
    puts("Piano Mode (press Q to go back):\n");
    puts("  [A S D F G H J] play notes\n");
    puts("  [Q] Back\n");

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
    __clear_screen();
    outb(0x21, pic1_mask);
}