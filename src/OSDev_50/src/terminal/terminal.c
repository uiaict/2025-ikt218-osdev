#include "../../include/terminal.h"
#include "../../include/libc/stdint.h"
#include "../../include/libc/stddef.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = 0x0F; // White on black
    terminal_buffer = VGA_MEMORY;
    
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = (uint16_t) ' ' | (uint16_t) terminal_color << 8;
        }
    }
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        const size_t index = terminal_row * VGA_WIDTH + terminal_column;
        terminal_buffer[index] = (uint16_t) data[i] | (uint16_t) terminal_color << 8;
        
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row = 0;
        }
    }
}

void terminal_writestring(const char* data) {
    size_t len = 0;
    while (data[len])
        len++;
    terminal_write(data, len);
}
