#include <stdint.h>
#include <stddef.h>
#include "terminal.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

static uint16_t* const terminal_buffer = (uint16_t*)VGA_ADDRESS;
static uint8_t terminal_row = 0;
static uint8_t terminal_col = 0;

static uint16_t make_vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

void cli() { asm volatile ("cli"); }
void sti() { asm volatile ("sti"); }

// Scrolls the screen up by one row
static void terminal_scroll() {
    cli();

    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[(y - 1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
        }
    }

    // Clear the last line
    for (int x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = make_vga_entry(' ', 0x07);
    }

    terminal_row = VGA_HEIGHT - 1;
    terminal_col = 0;

    sti();
}


// Prints a character at the current cursor location
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_row++;
        terminal_col = 0;
    } else if (c == '\r') {
        terminal_col = 0; // move to the start of the line
    } else if (c == '\b') {
        if (terminal_col > 0) {
            // Move cursor back one column, overwrite the character with a space
            terminal_col--;
            size_t index = terminal_row * VGA_WIDTH + terminal_col;
            terminal_buffer[index] = make_vga_entry(' ', 0x07); // Overwrite with space
        } else if (terminal_row > 0) {
            // Handle case when we're at the start of the line but not at the top row
            terminal_row--;
            terminal_col = VGA_WIDTH - 1;
            size_t index = terminal_row * VGA_WIDTH + terminal_col;
            terminal_buffer[index] = make_vga_entry(' ', 0x07); // Overwrite with space
        }
    } else {
        size_t index = terminal_row * VGA_WIDTH + terminal_col;
        terminal_buffer[index] = make_vga_entry(c, 0x07); // Write character
        terminal_col++;

        if (terminal_col >= VGA_WIDTH) {
            terminal_col = 0;
            terminal_row++;
        }
    }

    if (terminal_row >= VGA_HEIGHT) {
        terminal_scroll();
    }
}

// Prints a string
void terminal_write(const char* str) {
    while (*str) {
        terminal_putchar(*str++);
    }
}

// Clears the screen
void terminal_initialize() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = make_vga_entry(' ', 0x07);
        }
    }
    terminal_row = 0;
    terminal_col = 0;
}

// Writes one character at specific location on screen
void terminal_putchar_at(char c, uint8_t x, uint8_t y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) return;
    terminal_buffer[y * VGA_WIDTH + x] = make_vga_entry(c, 0x07);
}
