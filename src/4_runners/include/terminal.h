#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/stddef.h"  // Use custom stddef.h

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xb8000

void terminal_write(const char* str);
void terminal_put_char(char c);

// Cursor manipulation functions
void terminal_set_cursor(int row, int col);
void terminal_get_cursor(int* row, int* col);

#endif // TERMINAL_H