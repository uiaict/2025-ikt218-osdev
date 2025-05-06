#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <stddef.h>

void terminal_initialize(void);
void terminal_writestring(const char* str);
void terminal_write(const char* data, size_t size);
void terminal_putchar(char c);
void terminal_writeint(int value);

void terminal_setcolor(uint8_t fg, uint8_t bg);
void terminal_setcursor(size_t x, size_t y);
void terminal_clear(void);

uint8_t get_color(uint8_t fg, uint8_t bg);
uint16_t create_vga_entry(char c, uint8_t color);

size_t strlen(const char* str);

#endif
