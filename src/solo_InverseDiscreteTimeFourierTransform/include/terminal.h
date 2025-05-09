#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/stdint.h"
#include "libc/stddef.h"

// VGA text mode constants
void terminal_init(void);

// Print a character to the terminal
void terminal_put(char c);

// Write null-terminated string to the terminal
void terminal_write(const char* str);

#endif /* TERMINAL_H */
