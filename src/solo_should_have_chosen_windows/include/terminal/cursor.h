#ifndef CURSOR_H
#define CURSOR_H

#include "libc/io.h"
#include "libc/stdbool.h"

// External boolean to indicate if old logs should be printed
extern bool old_logs;
extern int cursor_position;


// Function to move the cursor to a new position
static inline void move_cursor(uint16_t position) {
    outb(0x3D4, 0x0F); // Select cursor low byte
    outb(0x3D5, (uint8_t)(position & 0xFF)); // Write low byte
    outb(0x3D4, 0x0E); // Select cursor high byte
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF)); // Write high byte
}

void clearTerminal(void);

#endif // CURSOR_H
