#pragma once

#include <libc/stdint.h>

#define COLOUR_BLACK 00
#define COLOUR_LIGHT_GRAY 07
#define COLOUR_WHITE 15

#define width 80
#define height 25

// void printf(int colour, const char* s, ...);
void scrollup();
void newLine();
void Reset();
void set_cursor_position(int x, int y);