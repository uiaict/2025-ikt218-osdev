// terminal.h
#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

// Terminal functions
void terminal_initialize(void);
void terminal_clear(void);
void terminal_set_color(uint8_t color);
uint8_t terminal_make_color(uint8_t fg, uint8_t bg);
void terminal_putchar(char c);
void terminal_write(const char* str);

// VGA color definitions
#define VGA_COLOR_BLACK         0
#define VGA_COLOR_BLUE          1
#define VGA_COLOR_GREEN         2
#define VGA_COLOR_CYAN          3
#define VGA_COLOR_RED           4
#define VGA_COLOR_MAGENTA       5
#define VGA_COLOR_BROWN         6
#define VGA_COLOR_LIGHT_GREY    7
#define VGA_COLOR_DARK_GREY     8
#define VGA_COLOR_LIGHT_BLUE    9
#define VGA_COLOR_LIGHT_GREEN   10
#define VGA_COLOR_LIGHT_CYAN    11
#define VGA_COLOR_LIGHT_RED     12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN   14
#define VGA_COLOR_WHITE         15

#endif // TERMINAL_H