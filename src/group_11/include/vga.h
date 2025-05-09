#ifndef VGA_H
#define VGA_H

#include <libc/stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// VGA text mode color constants
#define VGA_COLOR_BLACK 0
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_BRIGHT_GREEN 10

// Function declarations with C linkage
void clear_screen(void);
void put_char_at(char c, int x, int y, uint8_t color);
void init_vga(void);

#ifdef __cplusplus
}
#endif

#endif