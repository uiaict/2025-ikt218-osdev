#include "drivers/vga.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
static uint8_t vga_color = 0x07;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)(color << 8);
}

void vga_set_color(uint8_t color) {
    vga_color = color;
}

void vga_put_char(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else {
        const uint16_t index = cursor_row * VGA_WIDTH + cursor_col;
        vga_buffer[index] = vga_entry(c, vga_color);
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= VGA_HEIGHT) {
        cursor_row = 0; // or implement scrolling
    }
}

void vga_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        vga_put_char(str[i]);
    }
}

void vga_clear() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = vga_entry(' ', vga_color);
    }
    cursor_row = 0;
    cursor_col = 0;
}
