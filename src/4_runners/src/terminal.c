#include "terminal.h"
#include "libc/stddef.h"  // For size_t
#include "libc/string.h"  // For strlen
#include "libc/stdint.h"
#include "io.h"

static int cursor_row = 0; // Global cursor row
static int cursor_col = 0; // Global cursor column

static void update_hardware_cursor() {
    uint16_t position = cursor_row * VGA_WIDTH + cursor_col;

    // Send the high byte of the cursor position
    outb(0x3D4, 0x0E);
    outb(0x3D5, (position >> 8) & 0xFF);

    // Send the low byte of the cursor position
    outb(0x3D4, 0x0F);
    outb(0x3D5, position & 0xFF);
}

void terminal_set_cursor(int row, int col) {
    if (row >= 0 && row < VGA_HEIGHT) {
        cursor_row = row;
    }
    if (col >= 0 && col < VGA_WIDTH) {
        cursor_col = col;
    }
    update_hardware_cursor(); // Update the hardware cursor
}


void terminal_get_cursor(int* row, int* col) {
    if (row) {
        *row = cursor_row;
    }
    if (col) {
        *col = cursor_col;
    }
}

void terminal_write(const char* str) {
    volatile char* video_memory = (char*)VGA_MEMORY;

    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\n') {
            cursor_row++;
            cursor_col = 0;
            if (cursor_row >= VGA_HEIGHT) {
                // Scroll up if we exceed the screen height
                for (int y = 1; y < VGA_HEIGHT; y++) {
                    for (int x = 0; x < VGA_WIDTH; x++) {
                        video_memory[(y - 1) * VGA_WIDTH * 2 + x * 2] =
                            video_memory[y * VGA_WIDTH * 2 + x * 2];
                        video_memory[(y - 1) * VGA_WIDTH * 2 + x * 2 + 1] =
                            video_memory[y * VGA_WIDTH * 2 + x * 2 + 1];
                    }
                }
                // Clear the last row
                for (int x = 0; x < VGA_WIDTH; x++) {
                    video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + x * 2] = ' ';
                    video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + x * 2 + 1] = 0x07;
                }
                cursor_row = VGA_HEIGHT - 1;
            }
            continue;
        }

        if (str[i] == '\b') {
            // Handle backspace
            if (cursor_col > 0) {
                cursor_col--;
            } else if (cursor_row > 0) {
                cursor_row--;
                cursor_col = VGA_WIDTH - 1;
            }
            video_memory[(cursor_row * VGA_WIDTH + cursor_col) * 2] = ' ';
            video_memory[(cursor_row * VGA_WIDTH + cursor_col) * 2 + 1] = 0x07;
            continue;
        }

        video_memory[(cursor_row * VGA_WIDTH + cursor_col) * 2] = str[i];
        video_memory[(cursor_row * VGA_WIDTH + cursor_col) * 2 + 1] = 0x07;
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
            if (cursor_row >= VGA_HEIGHT) {
                // Scroll up if we exceed the screen height
                for (int y = 1; y < VGA_HEIGHT; y++) {
                    for (int x = 0; x < VGA_WIDTH; x++) {
                        video_memory[(y - 1) * VGA_WIDTH * 2 + x * 2] =
                            video_memory[y * VGA_WIDTH * 2 + x * 2];
                        video_memory[(y - 1) * VGA_WIDTH * 2 + x * 2 + 1] =
                            video_memory[y * VGA_WIDTH * 2 + x * 2 + 1];
                    }
                }
                // Clear the last row
                for (int x = 0; x < VGA_WIDTH; x++) {
                    video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + x * 2] = ' ';
                    video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + x * 2 + 1] = 0x07;
                }
                cursor_row = VGA_HEIGHT - 1;
            }
        }
    }
}


void terminal_put_char(char c) {
    volatile char* video_memory = (char*)VGA_MEMORY;

    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
        if (cursor_row >= VGA_HEIGHT) {
            // Scroll up if we exceed the screen height
            for (int y = 1; y < VGA_HEIGHT; y++) {
                for (int x = 0; x < VGA_WIDTH; x++) {
                    video_memory[(y - 1) * VGA_WIDTH * 2 + x * 2] =
                        video_memory[y * VGA_WIDTH * 2 + x * 2];
                    video_memory[(y - 1) * VGA_WIDTH * 2 + x * 2 + 1] =
                        video_memory[y * VGA_WIDTH * 2 + x * 2 + 1];
                }
            }
            // Clear the last row
            for (int x = 0; x < VGA_WIDTH; x++) {
                video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + x * 2] = ' ';
                video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + x * 2 + 1] = 0x07;
            }
            cursor_row = VGA_HEIGHT - 1;
        }
        update_hardware_cursor(); // Update the hardware cursor
        return;
    }

    if (c == '\b') {
        // Handle backspace
        if (cursor_col > 0) {
            cursor_col--;
        } else if (cursor_row > 0) {
            cursor_row--;
            cursor_col = VGA_WIDTH - 1;
        }
        video_memory[(cursor_row * VGA_WIDTH + cursor_col) * 2] = ' ';
        video_memory[(cursor_row * VGA_WIDTH + cursor_col) * 2 + 1] = 0x07;
        update_hardware_cursor(); // Update the hardware cursor
        return;
    }

    video_memory[(cursor_row * VGA_WIDTH + cursor_col) * 2] = c;
    video_memory[(cursor_row * VGA_WIDTH + cursor_col) * 2 + 1] = 0x07;
    cursor_col++;
    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= VGA_HEIGHT) {
            // Scroll up if we exceed the screen height
            for (int y = 1; y < VGA_HEIGHT; y++) {
                for (int x = 0; x < VGA_WIDTH; x++) {
                    video_memory[(y - 1) * VGA_WIDTH * 2 + x * 2] =
                        video_memory[y * VGA_WIDTH * 2 + x * 2];
                    video_memory[(y - 1) * VGA_WIDTH * 2 + x * 2 + 1] =
                        video_memory[y * VGA_WIDTH * 2 + x * 2 + 1];
                }
            }
            // Clear the last row
            for (int x = 0; x < VGA_WIDTH; x++) {
                video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + x * 2] = ' ';
                video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH * 2 + x * 2 + 1] = 0x07;
            }
            cursor_row = VGA_HEIGHT - 1;
        }
    }
    update_hardware_cursor(); // Update the hardware cursor
}
