#include "terminal.h"
#include <stdarg.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Pointer to the VGA framebuffer
static uint16_t* const vga_buffer = (uint16_t*) VGA_ADDRESS;
static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0x07; // Default color: light grey on black


// Create a VGA entry from a character and color
static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

// Initialize the terminal screen
void terminal_initialize(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
}

// Write a buffer of characters to the terminal
void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        char c = data[i];
        if (c == '\n') {
            terminal_row++;
            terminal_column = 0;
        } else {
            const size_t index = terminal_row * VGA_WIDTH + terminal_column;
            vga_buffer[index] = vga_entry(c, terminal_color);
            terminal_column++;
            if (terminal_column >= VGA_WIDTH) {
                terminal_column = 0;
                terminal_row++;
            }
        }
    }
}

// Write a formatted string to the terminal (supports %s and %d)
void terminal_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (size_t i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%' && fmt[i+1] == 's') {
            const char* str = va_arg(args, const char*);
            while (*str) {
                terminal_putchar(*str++);
            }
            i++;   // Skip 's'
        } else if (fmt[i] == '%' && fmt[i+1] == 'd') {
            int num = va_arg(args, int);
            char buffer[32];
            int pos = 0;
            if (num == 0) {
                terminal_putchar('0');
            } else {
                if (num < 0) {
                    terminal_putchar('-');
                    num = -num;
                }
                while (num > 0) {
                    buffer[pos++] = '0' + (num % 10);
                    num /= 10;
                }
                for (int j = pos - 1; j >= 0; j--) {
                    terminal_putchar(buffer[j]);
                }
            }
            i++;   // Skip 'd'
        } else {
            terminal_putchar(fmt[i]);
        }
    }

    va_end(args);
}

// Output a single character to the terminal
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
        return;
    }

    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    vga_buffer[index] = vga_entry(c, terminal_color);
    terminal_column++;
    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
}

