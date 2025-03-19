#include "libc/stdint.h"
#include "terminal/cursor.h"

#define VGA_ADDRESS 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define GREEN_ON_BLACK 0x02

// Used to store the current cursor position
static uint16_t cursor_position = 0;

// Function to print a single character at the current cursor position
static void print_char(char c) {
    // Points to the start of video memory text buffer
    uint16_t *video_memory = (uint16_t*) VGA_ADDRESS;

    // Handle newline manually
    if (c == '\n') {
        cursor_position += SCREEN_WIDTH - (cursor_position % SCREEN_WIDTH);
    } else {
        video_memory[cursor_position] = (GREEN_ON_BLACK << 8) | c;
        cursor_position++;
    }
    
    move_cursor(cursor_position);
}

// Printf function to print a string
void printf(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        print_char(str[i]);
        i++;
    }
}
