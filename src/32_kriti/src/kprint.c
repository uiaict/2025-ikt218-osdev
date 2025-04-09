#include <libc/stdint.h>
#include "kprint.h"

#define VIDEO_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static volatile char *video = (volatile char*)VIDEO_MEMORY;
static int cursor_pos = 0;

// Function to scroll the screen one line up
void scroll_screen() {
    // Move all lines up one line
    for (int y = 0; y < VGA_HEIGHT - 1; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            int dst = y * VGA_WIDTH + x;
            int src = (y + 1) * VGA_WIDTH + x;
            video[dst * 2] = video[src * 2];
            video[dst * 2 + 1] = video[src * 2 + 1];
        }
    }
    
    // Clear the last line
    for (int x = 0; x < VGA_WIDTH; x++) {
        int pos = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        video[pos * 2] = ' ';
        video[pos * 2 + 1] = 0x07;
    }
    
    // Move cursor to beginning of last line
    cursor_pos = (VGA_HEIGHT - 1) * VGA_WIDTH;
}

void kprint(const char *str) {
    while (*str) {
        if (*str == '\n') {
            // Move to the start of the next line
            cursor_pos = (cursor_pos / VGA_WIDTH + 1) * VGA_WIDTH;
        } else if (*str == '\b') {
            // Handle backspace
            if (cursor_pos > 0) {
                cursor_pos--;
                video[cursor_pos * 2] = ' ';
            }
        } else {
            // Write character
            video[cursor_pos * 2] = *str;       // Character
            video[cursor_pos * 2 + 1] = 0x07;   // White text on black background
            cursor_pos++;
        }
        
        // Check if we need to scroll
        if (cursor_pos >= VGA_WIDTH * VGA_HEIGHT) {
            scroll_screen();
        }
        
        str++;
    }
}

void kprint_hex(unsigned long num) {
    const char hex_chars[] = "0123456789ABCDEF";
    char buffer[19] = "0x0000000000000000";
    
    for (int i = 17; i >= 2; i--) {
        buffer[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    
    kprint(buffer);
}

// Function to print a decimal number
void kprint_dec(unsigned long num) {
    char buffer[21]; // Enough for 64-bit unsigned integer plus null terminator
    int i = 0;
    
    // Handle the case when num is 0
    if (num == 0) {
        kprint("0");
        return;
    }
    
    // Convert number to string (in reverse)
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    // Print in correct order
    while (i > 0) {
        char c[2] = {buffer[--i], '\0'};
        kprint(c);
    }
}

// Clear the screen
void kprint_clear() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = 0x07;
    }
    cursor_pos = 0;
}

// Set the position for the next character
void kprint_set_position(int x, int y) {
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        cursor_pos = y * VGA_WIDTH + x;
    }
}