#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "terminal.h"
#include "port_io.h"

////////////////////////////////////////
// VGA Text Mode Configuration
////////////////////////////////////////

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

static size_t terminal_row    = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0x0F;

#define VGA_PORT_CTRL  0x3D4
#define VGA_PORT_DATA  0x3D5

////////////////////////////////////////
// VGA Hardware Cursor
////////////////////////////////////////

// Set hardware cursor to given position
static inline void vga_set_hw_cursor(uint16_t pos) {
    outb(VGA_PORT_CTRL, 0x0F);
    outb(VGA_PORT_DATA, pos & 0xFF);
    outb(VGA_PORT_CTRL, 0x0E);
    outb(VGA_PORT_DATA, (pos >> 8) & 0xFF);
}

// Update hardware cursor from current row/column
static inline void terminal_update_cursor(void) {
    vga_set_hw_cursor((uint16_t)(terminal_row * VGA_WIDTH + terminal_column));
}

////////////////////////////////////////
// Terminal Core Functions
////////////////////////////////////////

// Initialize terminal and clear screen
void terminal_initialize(void) {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
        VGA_MEMORY[i] = (uint16_t)terminal_color << 8 | ' ';

    terminal_row = terminal_column = 0;
    terminal_update_cursor();
}

// Clear the screen and reset cursor position
void terminal_clear(void) {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
        VGA_MEMORY[i] = (uint16_t)terminal_color << 8 | ' ';

    terminal_row = terminal_column = 0;
    terminal_update_cursor();
}

// Set the current text color
void terminal_set_color(uint8_t color) {
    terminal_color = color;
}

// Alias for terminal_set_color (for compatibility)
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// Get the current terminal color value
uint8_t terminal_getcolor(void) {
    return terminal_color;
}

// Create VGA color attribute from fg and bg values
uint8_t terminal_make_color(uint8_t fg, uint8_t bg) {
    return fg | (bg << 4);
}

// Write character at (x, y) with specified color
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    VGA_MEMORY[index] = (uint16_t)color << 8 | c;
}

// Set terminal cursor to specific coordinates
void terminal_setcursor(size_t x, size_t y) {
    terminal_column = x;
    terminal_row = y;
    terminal_update_cursor();
}

// Print a single character using terminal_write
void terminal_putchar(char c) {
    char str[2] = { c, '\0' };
    terminal_write(str);
}

////////////////////////////////////////
// Terminal Text Output
////////////////////////////////////////

void terminal_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; ++i) {
        char c = str[i];

        if (c == '\n') {
            // Newline: move cursor to next line start
            terminal_column = 0;
            ++terminal_row;
        } 
        else if (c == '\b') {
            // Backspace: move cursor back and erase character
            if (terminal_column)
                --terminal_column;
            else if (terminal_row) {
                --terminal_row;
                terminal_column = VGA_WIDTH - 1;
            }

            // Replace erased character with blank
            VGA_MEMORY[terminal_row * VGA_WIDTH + terminal_column] =
                (uint16_t)terminal_color << 8 | ' ';
        } 
        else {
            // Normal character: write to screen and move cursor forward
            VGA_MEMORY[terminal_row * VGA_WIDTH + terminal_column] =
                (uint16_t)terminal_color << 8 | c;

            // Wrap line if end of row is reached
            if (++terminal_column >= VGA_WIDTH) {
                terminal_column = 0;
                ++terminal_row;
            }
        }

        // Handle scrolling when reaching bottom of screen
        if (terminal_row >= VGA_HEIGHT) {
            // Move all lines up by one (simple text-mode scroll)
            for (size_t row = 1; row < VGA_HEIGHT; ++row)
                for (size_t col = 0; col < VGA_WIDTH; ++col)
                    VGA_MEMORY[(row - 1) * VGA_WIDTH + col] =
                        VGA_MEMORY[row * VGA_WIDTH + col];

            // Clear last line
            for (size_t col = 0; col < VGA_WIDTH; ++col)
                VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + col] =
                    (uint16_t)terminal_color << 8 | ' ';

            terminal_row = VGA_HEIGHT - 1;
        }
    }

    // Always update hardware cursor after rendering
    terminal_update_cursor();
}