#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

void terminal_initialize();
void terminal_write(const char* str);
void terminal_putchar(char c);
void terminal_putchar_at(char c, uint8_t x, uint8_t y);

#endif