#include "libc/stdint.h"
#include "arch/i386/console.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* video_memory = (uint16_t*) VGA_ADDRESS;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

void console_clear() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_memory[i] = (0x07 << 8) | ' ';
    }
    cursor_row = cursor_col = 0;
}

void console_write_char(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else {
        int index = cursor_row * VGA_WIDTH + cursor_col;
        video_memory[index] = (0x07 << 8) | c;
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }
    // No scrolling yet
}

void console_write(const char* str) {
    while (*str) {
        console_write_char(*str++);
    }
}