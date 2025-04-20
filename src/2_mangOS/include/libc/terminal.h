
// void terminal_setcolor(uint8_t color);
// void terminal_putchar(char c);
// void terminal_writestring(const char *str);
// void terminal_clear();

// Combines background and foreground into one byte

#pragma once
#ifndef TERMINAL_H
#define TERMINAL_H

#include "common.h"
#include "libc/stdint.h"
#include "libc/stddef.h"
#define EOF (-1)

enum vga_color
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_PINK = 13,
    VGA_COLOR_YELLOW = 14,
    VGA_COLOR_WHITE = 15,
};

void terminal_initialize();
void terminal_setcolor(enum vga_color color);
;
// void monitor_putentryat(char c, uint8_t color, size_t x, size_t y);

void terminal_put(char c);
void terminal_clear();
void terminal_write(const char *data, size_t size);
void terminal_write_hex(uint32_t n);
void terminal_write_dec(uint32_t n);

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | (bg << 4);
}

#endif // MONITOR_H