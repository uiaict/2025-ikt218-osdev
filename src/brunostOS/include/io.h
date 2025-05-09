#ifndef IO_H
#define IO_H

#include "libc/stdint.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

enum vga_color {
	VGA_BLACK = 0,
	VGA_BLUE = 1,
	VGA_GREEN = 2,
	VGA_CYAN = 3,
	VGA_RED = 4,
	VGA_MAGENTA = 5,
	VGA_BROWN = 6,
	VGA_LIGHT_GREY = 7,
	VGA_GREY = 8,
	VGA_LIGHT_BLUE = 9,
	VGA_LIGHT_GREEN = 10,
	VGA_LIGHT_CYAN = 11,
	VGA_LIGHT_RED = 12,
	VGA_LIGHT_MAGENTA = 13,
	VGA_LIGHT_BROWN = 14,
	VGA_WHITE = 15,
};


// https://wiki.osdev.org/Inline_Assembly/Examples

void set_vga_color(enum vga_color, enum vga_color);
enum vga_color get_vga_txt_clr();
enum vga_color get_vga_bg_clr();

void enable_cursor(uint8_t, uint8_t); // non-functional visual cursor
void disable_cursor(); // non-functional visual cursor
void update_cursor(); // non-functional visual cursor

void clear_terminal();
void reset_cursor_pos();


void outb(uint16_t, uint8_t);
uint8_t inb(uint16_t port);


#endif // IO_H