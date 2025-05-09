#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/stdint.h"

void terminal_init();
void terminal_write(const char* str);
void terminal_backspace();

// Matrix rain
void terminal_putchar_at(char c, uint8_t x, uint8_t y);
void terminal_putchar_color_at(char c, uint8_t x, uint8_t y, uint8_t color);

#endif
