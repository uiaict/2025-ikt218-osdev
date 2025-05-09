#ifndef VGA_GRAPHICS_H
#define VGA_GRAPHICS_H

#include "libc/stdint.h"
#include "vga.h"

// VGA graphics mode constants
#define VGA_MODE_TEXT  0
#define VGA_MODE_13H   1

// Screen dimensions for Mode 13h (320x200)
#define GRAPHICS_WIDTH  320
#define GRAPHICS_HEIGHT 200

// Current video mode (declared externally)
extern uint8_t current_video_mode;

// Function prototypes
void set_mode_13h();
void set_mode_text();
void plot_pixel(uint16_t x, uint16_t y, uint8_t color);
void draw_hline(uint16_t x, uint16_t y, uint16_t length, uint8_t color);
void draw_vline(uint16_t x, uint16_t y, uint16_t length, uint8_t color);
void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
void fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
void draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint8_t color);
void fill_circle(uint16_t x, uint16_t y, uint16_t radius, uint8_t color);
void clear_screen_graphics(uint8_t color);
void draw_char_graphics(uint16_t x, uint16_t y, char c, uint8_t color);
void draw_string_graphics(uint16_t x, uint16_t y, const char* str, uint8_t color);

#endif // VGA_GRAPHICS_H