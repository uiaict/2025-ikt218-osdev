#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

#define VGA_BLINK 0x80

// VGA color enum
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

// Terminal state variables
extern size_t terminal_row;
extern size_t terminal_column;
extern uint8_t terminal_color;
extern uint16_t* terminal_buffer;

// Terminal functions
void terminal_initialize(void);
void terminal_set_color(uint8_t color);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_write_colored(const char* data, enum vga_color fg, enum vga_color bg);
void terminal_putchar(char c);
void update_cursor(int row, int col);

// Helper functions
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t vga_entry(unsigned char uc, uint8_t color);

#endif /* TERMINAL_H */