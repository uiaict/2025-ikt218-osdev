#include "terminal.h"

static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000; // VGA text buffer base
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

// Combines a character and a color into a VGA entry
uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

// Sets the default color: light grey on black
uint8_t make_color(uint8_t fg, uint8_t bg) {
    return fg | bg << 4;
}

// Initializes the terminal by clearing the screen
void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

// Writes a single character at the current cursor position
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;
        return;
    }

    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[index] = vga_entry(c, terminal_color);

    terminal_column++;
    if (terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
}

// Writes a null-terminated string to the terminal
void terminal_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}