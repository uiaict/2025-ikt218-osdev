#include "libc/terminal.h"
#include "libc/system.h"
#include "libc/stdarg.h"
#include "libc/stdint.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

static uint16_t* vga_buffer = (uint16_t*) VGA_ADDRESS;
static uint16_t terminal_row = 0;
static uint16_t terminal_column = 0;
static uint8_t terminal_color = 0x07; // Light grey on black

// Write a character and color into VGA memory
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | ((uint16_t) color << 8);
}

// Move hardware cursor
static void move_cursor() {
    uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Scroll screen up by one line
static void scroll() {
    uint8_t attributeByte = (0 << 4) | (7 & 0x0F); // Black bg, light grey text
    uint16_t blank = 0x20 | (attributeByte << 8); // Space character

    if (terminal_row >= VGA_HEIGHT) {
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }

        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            vga_buffer[i] = blank;
        }

        terminal_row = VGA_HEIGHT - 1;
    }
}

// Initialize terminal
void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = 0x07; // Light grey on black
    vga_buffer = (uint16_t*) VGA_ADDRESS;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
    }

    move_cursor();
}

// Put a single character on screen
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else {
        vga_buffer[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(c, terminal_color);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }

    scroll();
    move_cursor();
}

// Write a string to the terminal
void terminal_write(const char* str) {
    while (*str) {
        terminal_putchar(*str++);
    }
}
