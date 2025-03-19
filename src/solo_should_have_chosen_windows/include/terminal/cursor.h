#ifndef CURSOR_H
#define CURSOR_H

#include "libc/stdint.h"

// Function to write to an I/O port (inline assembly)
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Function to move the cursor to a new position
static inline void move_cursor(uint16_t position) {
    outb(0x3D4, 0x0F); // Select cursor low byte
    outb(0x3D5, (uint8_t)(position & 0xFF)); // Write low byte
    outb(0x3D4, 0x0E); // Select cursor high byte
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF)); // Write high byte
}

#endif // CURSOR_H
