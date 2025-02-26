#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

#define VGA_BLINK 0x80




// Move enum definition to header so it's visible to other files
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

// Add color-related function declarations
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
void terminal_set_color(uint8_t color);
void terminal_write_colored(const char* data, enum vga_color fg, enum vga_color bg);
// Original function declarations
void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
#endif