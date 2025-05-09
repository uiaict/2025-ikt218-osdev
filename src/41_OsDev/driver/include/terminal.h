#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <stddef.h>

////////////////////////////////////////
// Terminal Control Interface
////////////////////////////////////////

// Initialize the terminal display
void terminal_initialize(void);

// Clear the entire terminal screen
void terminal_clear(void);

// Set the current text color
void terminal_set_color(uint8_t color);

// Compatibility alias for setting color
void terminal_setcolor(uint8_t color);

// Get the current terminal color value
uint8_t terminal_getcolor(void);

// Combine foreground and background color into VGA attribute
uint8_t terminal_make_color(uint8_t fg, uint8_t bg);

// Output a character at a specific position with color
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

// Set cursor to given coordinates
void terminal_setcursor(size_t x, size_t y);

// Output a single character at current cursor position
void terminal_putchar(char c);

// Output a null-terminated string at current cursor position
void terminal_write(const char* str);

////////////////////////////////////////
// VGA Color Constants
////////////////////////////////////////

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
