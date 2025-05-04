// include/kernel/boot_art.h
#pragma once

#include "libc/stdint.h"   // in-tree integer types
#include <libc/stddef.h>
#include "pit.h"           // for optional interrupt-based delay

// ─── Combined init messages + ASCII art ──────────────────────────────────────

// Total lines: 7 init + 1 blank + 15 art = 23
#define BOOT_ART_LINES 23
static const char* boot_art[BOOT_ART_LINES] = {
    // Initializer messages
    "GDT loaded",
    "Kernel memory initialized",
    "Paging enabled",
    "PIT initialized",
    "IDT initialized",
    "IRQ initialized",
    "Keyboard controller initialized",
    "",  // blank line separates init from art

    // "Welcome to the byte of 33" ASCII art (15 lines)
    "     __          __  _                            _______               ",
    "     \\ \\        / / | |                          |__   __|              ",
    "      \\ \\  /\\  / /__| | ___ ___  _ __ ___   ___     | | ___           ",
    "       \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\    | |/ _ \\          ",
    "        \\  /\\  /  __/ | (_| (_) | | | | | |  __/    | | (_) |         ",
    "         \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|    |_|\\___/        ",
    "                                                                          ",
    "  _______ _            ____        _                __   ____    ____    ",
    " |__   __| |          |  _ \\      | |              / _| |___ \\  |___ \\   ",
    "    | |  | |__   ___  | |_) |_   _| |_ ___    ___ | |_    __) |   __) |  ",
    "    | |  | '_ \\ / _ \\ |  _ <| | | | __/ _ \\  / _ \\|  _|  |__ <|  |__ <|   ",
    "    | |  | | | |  __/ | |_) | |_| | ||  __/ | (_) | |    ___) |  ___) |  ",
    "    |_|  |_| |_|\\___| |____/ \\__, |\\__\\___|  \\___/|_|   |____/  |____/   ",
    "                              __/ |                                      ",
    "                             |___/                                       "
};

// ─── Low-level VGA helpers ────────────────────────────────────────────────────
static inline void __clear_screen(void) {
    volatile uint16_t* VGA = (uint16_t*)0xB8000;
    const uint8_t ATTR = 0x0E; // yellow on black
    for (int i = 0; i < 80*25; i++)
        VGA[i] = (ATTR << 8) | ' ';
}

static inline void __draw_text(int row, const char* text) {
    volatile uint16_t* VGA = (uint16_t*)0xB8000;
    const uint8_t ATTR = 0x0E;
    for (int col = 0; col < 80 && text[col]; col++) {
        VGA[row*80 + col] = (ATTR << 8) | (uint8_t)text[col];
    }
}

// Draw a simple ASCII progress bar at bottom
static inline void __draw_progress(int current, int total) {
    volatile uint16_t* VGA = (uint16_t*)0xB8000;
    const uint8_t ATTR = 0x0E;
    const int width = 50;
    const int pct_row = 23;
    const int bar_row = 24;
    int start = (80 - width) / 2;
    // Percentage text
    int pct = (current * 100) / total;
    char buf[5]; int len = 0;
    buf[len++] = (char)('0' + (pct/100)%10);
    buf[len++] = (char)('0' + (pct/10)%10);
    buf[len++] = (char)('0' + pct%10);
    buf[len++] = '%';
    buf[len] = '\0';
    int pct_col = (80 - len) / 2;
    for (int i = 0; i < len; i++) {
        VGA[pct_row*80 + pct_col + i] = (ATTR<<8) | buf[i];
    }
    // Bar outline
    VGA[bar_row*80 + start] = (ATTR<<8) | '[';
    VGA[bar_row*80 + start + width -1] = (ATTR<<8) | ']';
    for (int i = 1; i < width-1; i++)
        VGA[bar_row*80 + start + i] = (ATTR<<8) | ' ';
    // Filled portion
    int filled = (current * (width-2) + total/2) / total;
    for (int i = 0; i < filled; i++)
        VGA[bar_row*80 + start + 1 + i] = (ATTR<<8) | '=';
}

// ─── Public API ─────────────────────────────────────────────────────────────

/// Animate splash: prints each line then updates progress bar
static inline void animate_boot_screen(void) {
    __clear_screen();
    for (int i = 0; i < BOOT_ART_LINES; i++) {
        __draw_text(i, boot_art[i]);
        __draw_progress(i+1, BOOT_ART_LINES);
        for (volatile uint32_t d = 0; d < 50000000; d++) __asm__ volatile("nop");
    }
    // final pause
    for (volatile uint32_t d = 0; d < 40000000; d++) __asm__ volatile("nop");
}
