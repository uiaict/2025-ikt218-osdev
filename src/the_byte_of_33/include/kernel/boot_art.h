// include/kernel/boot_art.h
#pragma once

#include "libc/stdint.h"   // in-tree integer types
#include <libc/stddef.h>
#include "pit.h"           // for sleep_interrupt if you switch to interrupt-based delay

// ─── The ASCII art itself ─────────────────────────────────────────────────────

#define BOOT_ART_LINES 16
static const char* boot_art[BOOT_ART_LINES] = {
    // "Welcome to" banner
    "     __          __  _                            _______               ",
    "     \\ \\        / / | |                          |__   __|              ",
    "      \\ \\  /\\  / /__| | ___ ___  _ __ ___   ___     | | ___           ",
    "       \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\    | |/ _ \\          ",
    "        \\  /\\  /  __/ | (_| (_) | | | | | |  __/    | | (_) |         ",
    "         \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|    |_|\\___/        ",
    "                                                                          ",
    // "The byte of 33" banner
    "  _______ _            ____        _                __   ____    ____    ",
    " |__   __| |          |  _ \\      | |              / _| |___ \\  |___ \\   ",
    "    | |  | |__   ___  | |_) |_   _| |_ ___    ___ | |_    __) |   __) |  ",
    "    | |  | '_ \\ / _ \\ |  _ <| | | | __/ _ \\  / _ \\|  _|  |__ <|  |__ <|   ",
    "    | |  | | | |  __/ | |_) | |_| | ||  __/ | (_) | |    ___) |  ___) |  ",
    "    |_|  |_| |_|\\___| |____/ \\__, |\\__\\___|  \\___/|_|   |____/  |____/   ",
    "                              __/ |                                      ",
    "                             |___/                                       "
};

// ─── Low-level VGA helper ────────────────────────────────────────────────────
static inline void __clear_screen(void) {
    volatile uint16_t* VGA = (uint16_t*)0xB8000;
    const uint8_t ATTR = 0x0E; // yellow on black
    for (int i = 0; i < 80*25; i++) {
        VGA[i] = (ATTR << 8) | ' ';
    }
}

static inline void __draw_line(int row) {
    volatile uint16_t* VGA = (uint16_t*)0xB8000;
    const uint8_t ATTR = 0x0E;
    const char* line = boot_art[row];
    for (int col = 0; col < 80 && line[col]; col++) {
        VGA[row*80 + col] = (ATTR << 8) | (uint8_t)line[col];
    }
}

// ─── Public API ─────────────────────────────────────────────────────────────

/// Animate splash, one line at a time with a slow delay
static inline void animate_boot_screen(void) {
    __clear_screen();
    for (int row = 0; row < BOOT_ART_LINES; row++) {
        __draw_line(row);
        // ~0.3s per line on typical emulator; adjust as needed
        for (volatile uint32_t i = 0; i < 1000; i++) {
            __asm__ volatile("nop");
        }
    }
}