#include "terminal.h"
#include "libc/stddef.h"
#include "libc/string.h"
#include "libc/stdint.h"
#include "io.h"

// Forward declarations of static functions
static void update_cursor(void);
static void terminal_newline(void);
static void terminal_backspace(void);
static void terminal_scroll(void);
static inline uint16_t vga_entry(char c, uint8_t color);

// Terminal state
static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4;
static uint16_t* terminal_buffer = (uint16_t*) VGA_MEMORY;

// Helper function to create a VGA entry
static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

// Hardware cursor control
static void update_cursor(void) {
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

// Scroll the terminal up by one line
static void terminal_scroll(void) {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t from_index = y * VGA_WIDTH + x;
            const size_t to_index = (y - 1) * VGA_WIDTH + x;
            terminal_buffer[to_index] = terminal_buffer[from_index];
        }
    }
    
    // Clear the last line
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
}

// Handle newline
static void terminal_newline(void) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
        terminal_scroll();
        terminal_row = VGA_HEIGHT - 1;
    }
    update_cursor();
}

// Handle backspace
static void terminal_backspace(void) {
    if (terminal_column > 0) {
        terminal_column--;
    } else if (terminal_row > 0) {
        terminal_row--;
        terminal_column = VGA_WIDTH - 1;
    }
    
    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[index] = vga_entry(' ', terminal_color);
    update_cursor();
}

// Public functions
void terminal_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
    update_cursor();
}

void terminal_initialize(void) {
    terminal_clear();
}

void terminal_set_cursor(int row, int col) {
    if (row >= 0 && row < VGA_HEIGHT && col >= 0 && col < VGA_WIDTH) {
        terminal_row = row;
        terminal_column = col;
        update_cursor();
    }
}

void terminal_get_cursor(int* row, int* col) {
    if (row) *row = terminal_row;
    if (col) *col = terminal_column;
}

void terminal_put_char(char c) {
    if (c == '\n') {
        terminal_newline();
        return;
    }
    
    if (c == '\b') {
        terminal_backspace();
        return;
    }

    // Check boundaries to prevent writing outside the screen
    if (terminal_row >= VGA_HEIGHT || terminal_column >= VGA_WIDTH) {
        return;
    }

    // Calculate the index in the VGA buffer
    const size_t index = terminal_row * VGA_WIDTH + terminal_column;

    // Write the character with the current color to the VGA buffer
    terminal_buffer[index] = vga_entry(c, terminal_color);

    // Move to the next column
    if (++terminal_column == VGA_WIDTH) {
        terminal_newline();
    }

    // Update the hardware cursor
    update_cursor();
}

void terminal_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_put_char(str[i]);
    }
}

void terminal_write_centered(int row, const char* str) {
    size_t len = strlen(str);
    if (len > VGA_WIDTH) len = VGA_WIDTH;
    size_t col = (VGA_WIDTH - len) / 2;
    terminal_set_cursor(row , col - 4 );
    terminal_write(str);
}

void terminal_write_at(int row, int col, const char* str) {
    terminal_set_cursor(row, col);
    terminal_write(str);
}

// Set color for terminal output
void terminal_set_color(uint8_t color) {
    terminal_color = color;
}

// Get current color
uint8_t terminal_get_color(void) {
    return terminal_color;
}