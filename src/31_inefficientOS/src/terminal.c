#include "terminal.h"
#include "common.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

// Make this function non-static so it's accessible from the header
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}

// Add this function to allow changing colors
void terminal_set_color(uint8_t color) {
    terminal_color = color;
}

// Add this function for colored text
void terminal_write_colored(const char* data, enum vga_color fg, enum vga_color bg) {
    uint8_t old_color = terminal_color;
    terminal_color = vga_entry_color(fg, bg);
    terminal_writestring(data);
    terminal_color = old_color;
}

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*)0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void update_cursor(int row, int col) {
    unsigned short position = (row * VGA_WIDTH) + col;
    
    // Output the cursor position to the CRT controller
    outb(0x3D4, 0x0F);                  // Tell the controller we're setting the low cursor byte
    outb(0x3D5, (unsigned char)(position & 0xFF)); // Send the low byte
    outb(0x3D4, 0x0E);                  // Tell the controller we're setting the high cursor byte
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF)); // Send the high byte
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
        if (terminal_row == VGA_HEIGHT)
            terminal_row = 0;
        return;
    } else if (c == '\b') {
        // Handle backspace
        if (terminal_column > 0) {
            terminal_column--;
            terminal_buffer[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(' ', terminal_color);
            update_cursor(terminal_row, terminal_column);
        }
        return;
    }
    
    terminal_buffer[terminal_row * VGA_WIDTH + terminal_column] = vga_entry(c, terminal_color);
   
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
    
    // Update cursor position
    update_cursor(terminal_row, terminal_column);
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++)
        terminal_putchar(data[i]);
}
