#include "terminal.h"
#include <libc/stddef.h>
#include <libc/stdint.h>

#define VGA_MEMORY (uint16_t*)0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25 // ADDED

static size_t terminal_row = 0; // ADDED
static size_t terminal_column = 0; // ADDED
static uint16_t* const vga_buffer = VGA_MEMORY; // ADDED

void terminal_initialize(void) {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) { // CHANGED
        vga_buffer[i] = (uint16_t) ' ' | (uint16_t) 0x0700;
    }
    terminal_row = 0; // ADDED
    terminal_column = 0; // ADDED
}

void terminal_putchar(char c) { // ADDED
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else {
        size_t index = terminal_row * VGA_WIDTH + terminal_column;
        vga_buffer[index] = (uint16_t) c | (uint16_t) 0x0700;
        terminal_column++;
        if (terminal_column >= VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }

    if (terminal_row >= VGA_HEIGHT) {
        for (size_t row = 1; row < VGA_HEIGHT; row++) {
            for (size_t col = 0; col < VGA_WIDTH; col++) {
                vga_buffer[(row - 1) * VGA_WIDTH + col] = vga_buffer[row * VGA_WIDTH + col];
            }
        }
        for (size_t col = 0; col < VGA_WIDTH; col++) {
            vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = ' ' | 0x0700;
        }
        terminal_row = VGA_HEIGHT - 1;
    }
}

void writeline(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) { // CHANGED
        terminal_putchar(str[i]); // CHANGED
    }
}
