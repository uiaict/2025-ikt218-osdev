#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/stddef.h"
#include "libc/stdint.h"

// VGA Colors
enum vga_color {
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
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// VGA specifics
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// Terminal functions
void terminal_initialize(void);
void terminal_clear(void);
void terminal_put_char(char c);
void terminal_write(const char* str);
void terminal_set_cursor(int row, int col);
void terminal_get_cursor(int* row, int* col);
void terminal_write_centered(int row, const char* str);
void terminal_write_at(int row, int col, const char* str);

// Color functions (new)
void terminal_set_color(uint8_t color);
uint8_t terminal_get_color(void);

#endif /* TERMINAL_H */