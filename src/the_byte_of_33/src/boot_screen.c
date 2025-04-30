// src/boot_screen.c

#include "libc/stdint.h"
#include "kernel/boot_art.h"    // <- note the “kernel/” prefix
#include "kernel/boot_screen.h" // <- our new prototype
#include "pit.h"                // for sleep_interrupt()

static volatile uint16_t* const VGA = (uint16_t*)0xB8000;
static const uint8_t ATTR = 0x1F;    // white on blue background

static void clear_screen(void) {
    for (int i = 0; i < 80*25; i++) {
        VGA[i] = (ATTR << 8) | ' ';
    }
}

static void draw_art_line(int row) {
    const char* line = boot_art[row];
    for (int col = 0; line[col] && col < 80; col++) {
        VGA[row*80 + col] = (ATTR << 8) | (uint8_t)line[col];
    }
}

void animate_boot_screen(void) {
    clear_screen();
    for (int row = 0; row < BOOT_ART_LINES; row++) {
        draw_art_line(row);
        sleep_interrupt(100);   // delay 100ms between lines
    }
}
