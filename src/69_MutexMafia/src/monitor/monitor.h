#ifndef MONITOR_H
#define MONITOR_H

#include <libc/system.h>
#include "../io/keyboard.h"
#include "../io/printf.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

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
	VGA_COLOR_YELLOW = 14,
	VGA_COLOR_WHITE = 15,
};

extern volatile char *video_memory;
extern uint16_t cursor;
extern uint8_t terminal_row;
extern uint8_t terminal_column;
extern uint8_t terminal_color;

void scroll();
void clear_screen();
void print_menu();
void print_mutexMafia();
void move_cursor();
void init_monitor();
void draw_char_at(int x, int y, char c, uint8_t color);

#endif