#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

// Initialize the terminal screen
void terminal_initialize(void);

void terminal_setcolor(uint8_t color);

// Write raw data to the terminal
void terminal_write(const char* data, size_t size);

// Write a single character to the terminal
void terminal_putchar(char c); 

// Write a formatted string to the terminal
void terminal_printf(const char* fmt, ...);


#endif
