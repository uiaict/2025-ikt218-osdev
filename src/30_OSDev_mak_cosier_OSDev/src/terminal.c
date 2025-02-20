#include "libc/stdint.h"
#include "../include/teminal.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define WHITE_ON_BLACK 0x0F

uint16_t* terminal_buffer = (uint16_t*)VGA_ADDRESS;
int term_row = 0;
int term_col = 0;

void terminal_putc(char c) {
    if (c == '\n') {
        term_row++;
        term_col = 0;
        return;
    }
    terminal_buffer[term_row * VGA_WIDTH + term_col] = (WHITE_ON_BLACK << 8) | c;
    term_col++;
}

void printf(const char* format, ...) {
    for (int i = 0; format[i] != '\0'; i++) {
        terminal_putc(format[i]);
    }
}
