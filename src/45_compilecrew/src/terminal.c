#include "libc/terminal.h"
#include "libc/stdarg.h"
#include "libc/stdint.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* vga_buffer = (uint16_t*) VGA_ADDRESS;
static uint16_t terminal_row = 0, terminal_column = 0;

// VGA uses color attributes: foreground (text) & background
static uint8_t vga_color = 0x07;  // Light gray text on black background

// Write a character to the screen
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else {
        vga_buffer[terminal_row * VGA_WIDTH + terminal_column] = (uint16_t)c | (vga_color << 8);
        terminal_column++;
        if (terminal_column >= VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }

    if (terminal_row >= VGA_HEIGHT) {
        terminal_row = 0; // Reset to the top (basic scrolling)
    }
}

// Write a string to the screen
void terminal_write(const char* str) {
    while (*str) {
        terminal_putchar(*str++);
    }
}
