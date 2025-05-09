#ifndef TERMINAL_H
#define TERMINAL_H

#include <libc/stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void terminal_initialize(void);
void writeline(const char* str);
void terminal_putchar(char c);
void update_cursor(int row, int col);
void terminal_backspace();
void printf(const char* format, ...);
void terminal_clear(void);

// Make these accessible for other modules (like keyboard handler)
extern size_t terminal_row;
extern size_t terminal_column;

#endif