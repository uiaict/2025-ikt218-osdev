#include "terminal.h"


#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

// VGA buffer is a 2-byte per character (char + color)
static uint16_t* const buffer = (uint16_t*)VGA_ADDRESS;
static size_t row = 0;
static size_t column = 0;
static uint8_t color = 0x0F;  // white on black

// Initialize  terminal
void terminal_init(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t idx = y * VGA_WIDTH + x;
            buffer[idx] = ((uint16_t) ' ' | (uint16_t) color << 8);
        }
    }
    row = 0;
    column = 0;
}

static void newline(void) {
    column = 0;
    if (++row == VGA_HEIGHT) {

        for (size_t y = 1; y < VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                buffer[(y - 1) * VGA_WIDTH + x] = buffer[y * VGA_WIDTH + x];
            }
        }

        for (size_t x = 0; x < VGA_WIDTH; x++) {
            buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ((uint16_t) ' ' | (uint16_t) color << 8);
        }
        row = VGA_HEIGHT - 1;
    }
}

// Print char to the terminal
void terminal_put(char c) {
    if (c == '\n') {
        newline();
        return;
    }
    const size_t idx = row * VGA_WIDTH + column;
    buffer[idx] = ((uint16_t) c | (uint16_t) color << 8);
    if (++column == VGA_WIDTH) {
        newline();
    }
}

// Write a string to the terminal
void terminal_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_put(str[i]);
    }
}
