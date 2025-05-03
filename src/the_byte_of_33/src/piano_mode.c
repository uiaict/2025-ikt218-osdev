#include "piano_mode.h"
#include "io.h"
#include "kernel/boot_art.h"
#include <libc/stdint.h>


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
    puts("  [Keys] Play notes\n");
    puts("  [Q] Back\n");

    uint8_t sc;
    do {
        sc = wait_make();
        // map scancodes to notes here
    } while (sc != 0x10);  // 'Q'

    __clear_screen();
    outb(0x21, pic1_mask);  // unmask IRQ1
}