#include "printing/terminal.h"
#include "libc/stdint.h"
#include "util.h"

#define VGA_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x0F
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* video_memory = (uint16_t*) VGA_ADDRESS;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

static void update_cursor() {
    uint16_t position = cursor_row * VGA_WIDTH + cursor_col;

    // Send the high byte of the position to port 0x3D4
    outb(0x3D4, 0x0E);
    outb(0x3D5, (position >> 8) & 0xFF);

    // Send the low byte of the position to port 0x3D4
    outb(0x3D4, 0x0F);
    outb(0x3D5, position & 0xFF);
}

void terminal_init() {
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i] = (WHITE_ON_BLACK << 8) | ' ';
    }
    cursor_row = 0;
    cursor_col = 0;
    update_cursor();
}

void terminal_backspace() {
    if (cursor_col > 0) {
        cursor_col--;
    } else if (cursor_row > 0) {
        cursor_row--;
        cursor_col = VGA_WIDTH - 1;
    }
    video_memory[cursor_row * VGA_WIDTH + cursor_col] = (WHITE_ON_BLACK << 8) | ' ';
    update_cursor();
}

void terminal_write(const char* str) {
    while (*str) {
        char c = *str++;
        if (c == '\n') {
            cursor_row++;
            cursor_col = 0;
            continue;
        } 
            
        video_memory[cursor_row * 80 + cursor_col] = (WHITE_ON_BLACK << 8) | c;
        cursor_col++;
        if (cursor_col >= 80) {
            cursor_col = 0;
            cursor_row++;
        }
        
        // Scrolling if cursor exceeds bounds
        if (cursor_row >= VGA_HEIGHT) {
            for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
                video_memory[i] = video_memory[i + VGA_WIDTH];
            }
            for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
                video_memory[i] = (WHITE_ON_BLACK << 8) | ' ';
            }
            cursor_row = VGA_HEIGHT - 1;
        }
    }

    // Update the cursor position
    update_cursor();
    
}

// Matrix rain
void terminal_putchar_at(char c, uint8_t x, uint8_t y) {
    uint16_t pos = y * VGA_WIDTH + x;
    video_memory[pos] = (WHITE_ON_BLACK << 8) | c;
}

void terminal_putchar_color_at(char c, uint8_t x, uint8_t y, uint8_t color) {
    uint16_t pos = y * 80 + x;
    video_memory[pos] = (color << 8) | c;
}
