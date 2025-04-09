#include "terminal/cursor.h"

#include "libc/stdint.h"

#define VGA_ADDRESS 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define WHITE_ON_BLACK 0x07

int cursor_position = 0; // Initialize cursor position
bool old_logs = false;

// Function to clear the terminal (clear screen and reset cursor)
void clearTerminal(void) {
    uint16_t *video_memory = (uint16_t*) VGA_ADDRESS;

    // Fill the entire screen with blank spaces
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i] = (WHITE_ON_BLACK << 8) | ' ';
    }

    // Reset cursor to (0,0)
    old_logs = false;
    cursor_position = 0;
    move_cursor(cursor_position);
}

