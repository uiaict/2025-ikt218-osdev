#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"
#include "libc/stdint.h"
#include "libc/stddef.h"

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
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW = 14, // Yellow
    VGA_COLOR_WHITE = 15,
};

void monitor_initialize();
void monitor_setcolor(uint8_t color);
void monitor_putentryat(char c, uint8_t color, size_t x, size_t y);

void move_cursor_direction(int move_x, int move_y);
void monitor_put(char c);
void monitor_clear();
void monitor_write(const char *data, size_t size);
void monitor_write_hex(uint32_t n);
void monitor_write_dec(uint32_t n);

#endif // MONITOR_H