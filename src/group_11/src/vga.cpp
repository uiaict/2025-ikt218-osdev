#include "vga.h"
#include "libc/stdlib.h"
#include "matrix_rain.h"

static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

extern "C" void clear_screen(void) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            const int index = y * SCREEN_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', 0x07);
        }
    }
}

extern "C" void put_char_at(char c, int x, int y, uint8_t color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        const int index = y * SCREEN_WIDTH + x;
        VGA_MEMORY[index] = vga_entry(c, color);
    }
}

extern "C" void init_vga(void) {
    clear_screen();
}