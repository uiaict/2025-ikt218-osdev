#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/stddef.h"

// Terminal functions
void terminal_init();
void terminal_putchar(char c);
void terminal_write(const char* str);

#endif // TERMINAL_H