#include "terminal.h"
#include <libc/stddef.h>
#include <libc/stdint.h>
#include "idt.h"  // For port I/O functions

#define VGA_MEMORY (uint16_t*)0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Make variables global and externally accessible
size_t terminal_row = 0;
size_t terminal_column = 0;
static uint16_t* const vga_buffer = VGA_MEMORY;

void terminal_initialize(void) {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (uint16_t) ' ' | (uint16_t) 0x0700;
    }
    terminal_row = 0;
    terminal_column = 0;
}

// Update the hardware cursor position
void update_cursor(int row, int col) {
    unsigned short position = (row * VGA_WIDTH) + col;
    
    // Tell the VGA controller we are setting the low cursor byte
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    
    // Tell the VGA controller we are setting the high cursor byte
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

// Implement backspace functionality
void terminal_backspace() {
    if (terminal_column > 0) {
        // Move back one column
        terminal_column--;
        // Clear the character
        vga_buffer[terminal_row * VGA_WIDTH + terminal_column] = (uint16_t)' ' | 0x0700;
        // Update cursor position
        update_cursor(terminal_row, terminal_column);
    } 
    else if (terminal_row > 0) {
        // Move to end of previous line
        terminal_row--;
        terminal_column = VGA_WIDTH - 1;
        // Clear the character
        vga_buffer[terminal_row * VGA_WIDTH + terminal_column] = (uint16_t)' ' | 0x0700;
        // Update cursor position
        update_cursor(terminal_row, terminal_column);
    }
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } 
    else if (c == '\b') {
        // Call backspace handler
        terminal_backspace();
        return; // Skip the rest of the function
    }
    else {
        size_t index = terminal_row * VGA_WIDTH + terminal_column;
        vga_buffer[index] = (uint16_t) c | (uint16_t) 0x0700;
        terminal_column++;
        if (terminal_column >= VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }

    // Handle scrolling
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
    
    // Update the hardware cursor position
    update_cursor(terminal_row, terminal_column);
}

void writeline(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}
