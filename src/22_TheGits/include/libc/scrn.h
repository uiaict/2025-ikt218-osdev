#ifndef SCRN_H
#define SCRN_H

#include "libc/stdint.h"
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Fargekoder for tekst og bakgrunn
#define VGA_COLOR(fg, bg) ((bg << 4) | (fg))
#define VGA_ENTRY(c, color) ((uint16_t)c | (uint16_t)color << 8)
// VGA-minneadresse
#define VGA_MEMORY (uint16_t*)0xB8000
#define VGA_WIDTH 80


void terminal_write(const char* str, uint8_t color);
void printf(const char* format, ...);

void panic(const char* message);

#endif // SCRN_H