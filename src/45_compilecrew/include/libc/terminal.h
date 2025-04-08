#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/stdint.h"

// Function to print a single character to the VGA text buffer
void terminal_putchar(char c);

// Function to print a string to the screen
void terminal_write(const char* str);

#endif // TERMINAL_H
