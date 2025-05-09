#include <libc/stdint.h>
#include "putchar.h"
#include "terminal.h"

// Hardware VGA text mode information
const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;
uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

// Terminal state
size_t terminal_row = 0;
size_t terminal_column = 0;
uint8_t terminal_color = 0;

// Convert color to VGA entry
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

// Convert character and color to VGA entry
uint16_t vga_entry(unsigned char c, uint8_t color) {
    return (uint16_t) c | (uint16_t) color << 8;
}

// Initialize terminal
void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    
    // Clear the screen
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', terminal_color);
        }
    }
}

// Set terminal color
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// Put entry at specific position
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    VGA_MEMORY[index] = vga_entry(c, color);
}

// Scroll the terminal up one line if needed
void terminal_scroll() {
    if (terminal_row >= VGA_HEIGHT) {
        // Move each line up by one
        for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                const size_t from_index = (y + 1) * VGA_WIDTH + x;
                const size_t to_index = y * VGA_WIDTH + x;
                VGA_MEMORY[to_index] = VGA_MEMORY[from_index];
            }
        }
        
        // Clear the bottom line
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', terminal_color);
        }
        
        terminal_row = VGA_HEIGHT - 1;
    }
}

// Set cursor position
void terminal_set_cursor_position(uint16_t row, uint16_t col) {
    // Set the internal cursor position variables
    terminal_row = row;
    terminal_column = col;
    
    // If your terminal has hardware cursor positioning, add that code here
    // For VGA text mode, this might involve writing to I/O ports 0x3D4 and 0x3D5
    // For now, we just set the internal position for the next write
}

// Put a single character
void putchar(char c) {
    // Handle newline
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
        terminal_scroll();
        return;
    }
    
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    
    // Handle cursor position
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
        terminal_scroll();
    }
}