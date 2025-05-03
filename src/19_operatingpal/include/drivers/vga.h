#pragma once
#include "libc/stdint.h"

void vga_clear();
void vga_put_char(char c);
void vga_write(const char* str);
void vga_set_color(uint8_t color);
