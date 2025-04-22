#include "terminal.h"
#include "libc/stdint.h"  
#include "libc/stddef.h" 

// VGA text mode memory address
#define VGA_MEMORY 0xB8000
// Terminal dimensions
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Terminal state
static uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;
static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0x0F; // White on black

// Initialize terminal
void terminal_init() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = (uint16_t)' ' | ((uint16_t)terminal_color << 8);
        }
    }
}

// Write a character to terminal
void terminal_putchar(char c) {
    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[index] = (uint16_t)c | ((uint16_t)terminal_color << 8);
   
    // Advance cursor
    terminal_column++;
    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
}


void terminal_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}