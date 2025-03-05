#include "terminal.h"
#include "libc/stdint.h"
#include "libc/stddef.h"

#define VGA_MEMORY (uint16_t*)0xB8000
#define VGA_WIDTH 80

void terminal_initialize(void) {
    uint16_t* vga_buffer = VGA_MEMORY;
    for (size_t i = 0; i < VGA_WIDTH * 25; i++) {
        vga_buffer[i] = (uint16_t) ' ' | (uint16_t) 0x0700;
    }
}

void terminal_writestring(const char* str) {
    uint16_t* vga_buffer = VGA_MEMORY;
    for (size_t i = 0; str[i] != '\0'; i++) {
        vga_buffer[i] = (uint16_t) str[i] | (uint16_t) 0x0700;
    }
}