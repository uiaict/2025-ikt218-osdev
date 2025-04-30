#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/stdint.h"

// Function to print a single character to the VGA text buffer
void terminal_putchar(char c);

// Function to print a string to the screen
void terminal_write(const char* str);

void terminal_clear();

void draw_front_page();

void disable_cursor();

void enable_cursor(uint8_t start, uint8_t end);

void draw_music_selection();
#endif // TERMINAL_H
