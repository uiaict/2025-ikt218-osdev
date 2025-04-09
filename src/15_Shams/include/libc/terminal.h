#ifndef TERMINAL_H
#define TERMINAL_H

#include <libc/stdint.h>
#include <libc/stddef.h> // for size_t

extern int cursor_x;
extern int cursor_y;

void terminal_putc(char c);
void terminal_write(const char *str);
void move_cursor();

#endif
