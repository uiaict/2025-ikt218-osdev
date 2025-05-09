#pragma once

#include "libc/stdint.h"

#define WIDTH 80
#define HEIGHT 25

#define DEFAULT_COLOR 15

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define PURPLE 5
#define BROWN 6
#define GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define YELLOW 14
#define WHITE 15

void terminal_initialize();
void terminal_clear();
void terminal_put(char c, int color, int x, int y);
void terminal_write(int color, const char *str);
void itoa(int num, char *str, int base);
void ftoa(float num, char *str, int afterpoint);

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(int x, int y);
