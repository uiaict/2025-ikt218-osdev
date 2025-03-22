#pragma once

#include "stdint.h"
#include "stdbool.h"

enum vga_color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN = 14,
	WHITE = 15,
};

void putchar_at(const char*, size_t, size_t);
void printf(const char*);
void set_vga_color(enum vga_color, enum vga_color);

// void reset_terminal();
// void resize_terminal();