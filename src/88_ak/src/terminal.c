#include <libc/stdint.h>
#include <libc/stddef.h>
#include "terminal.h"

static volatile uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static size_t terminal_row = 0;
static size_t terminal_col = 0;
static uint8_t terminal_color = (0 /*bg*/ << 4) | (15 /*fg*/);

void terminal_set_color(uint8_t fg, uint8_t bg) {
    terminal_color = (bg << 4) | (fg & 0x0F);
}

void terminal_initialize(void) {
    for (size_t y = 0; y < 25; y++) {
        for (size_t x = 0; x < 80; x++) {
            VGA_BUFFER[y * 80 + x] = ((uint16_t)terminal_color << 8) | ' ';
        }
    }
    terminal_row = terminal_col = 0;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_col = 0;
        terminal_row++;
    } else {
        VGA_BUFFER[terminal_row * 80 + terminal_col] =
            (uint16_t)terminal_color << 8 | (uint8_t)c;
        terminal_col++;
        if (terminal_col >= 80) {
            terminal_col = 0;
            terminal_row++;
        }
    }
    if (terminal_row >= 25) {
        terminal_row = 0;  // eller scroll om du vil
    }
}

void terminal_write(const char* str) {
    for (size_t i = 0; str[i]; i++) {
        terminal_putchar(str[i]);
    }
}

void terminal_write_hex(uint32_t v) {
    static const char* hex = "0123456789ABCDEF";
    terminal_write("0x");
    for (int shift = 28; shift >= 0; shift -= 4) {
        uint8_t nib = (v >> shift) & 0xF;
        terminal_putchar(hex[nib]);
    }
}
