#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "terminal.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

void terminal_initialize(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = (uint8_t) ' ' | 0x0F00; // ' ' + light gray on black
    }
}

void terminal_write(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        VGA_MEMORY[i] = (uint8_t) str[i] | 0x0F00;
    }
}
