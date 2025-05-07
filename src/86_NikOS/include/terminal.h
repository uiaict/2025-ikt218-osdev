#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <stddef.h>

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
	VGA_COLOR_YELLOW = 14,
	VGA_COLOR_WHITE = 15,
};

void terminal_initialize(void);

void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* str);
void terminal_writeint(int32_t value);
void terminal_writeuint(uint32_t value);

void terminal_putchar_color(char c, uint8_t color);
void terminal_write_color(const char* data, size_t size, uint8_t color);
void terminal_writestring_color(const char* data, uint8_t color);
void terminal_writeint_color(int32_t value, uint8_t color);
void terminal_writeuint_color(uint32_t value, uint8_t color);

void terminal_setcolor(uint8_t fg, uint8_t bg);
void terminal_setcursor(size_t x, size_t y);
void terminal_clear(void);
void clear_input_line(void);

void terminal_hello(void);

size_t terminal_get_column(void);
size_t terminal_get_row(void);

uint8_t get_color(uint8_t fg, uint8_t bg);
uint16_t create_vga_entry(char c, uint8_t color);

#endif
