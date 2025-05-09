#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "libc/stdint.h"

// Terminal position variables
extern size_t terminal_row;
extern size_t terminal_column;

// VGA hardware text mode color constants
typedef enum vga_color {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
    
    // Extra color combinations for special cases
    COLOR_YELLOW = COLOR_LIGHT_BROWN,  // Yellow text on black background
    COLOR_GRAY = COLOR_LIGHT_GREY,     // Alias for light grey
    COLOR_BLACK_ON_WHITE = 0xF0,       // Black text on white background
    COLOR_BLACK_ON_GREEN = 0x20,       // Black text on green background
    COLOR_BLACK_ON_BLUE = 0x10,        // Black text on blue background
    COLOR_BLACK_ON_RED = 0x40          // Black text on red background
} vga_color_t;

// Display functions
void display_initialize(void);
void display_putchar(char c);
void display_write(const char* data);
void display_writestring(const char* data);
void display_write_char(char c);
void display_write_char_color(char c, vga_color_t color);
void display_write_string(const char* str);
void display_write_color(const char* str, vga_color_t color);
void display_write_decimal(int num);
void display_write_hex(uint32_t num);
void display_move_cursor(void);
void display_clear(void);
void display_set_color(vga_color_t fg, vga_color_t bg);
void display_boot_logo(void);  // Display the boot logo

// Cursor positioning functions
void display_set_cursor(size_t x, size_t y);
void display_hide_cursor(void);

#endif /* _DISPLAY_H */ 