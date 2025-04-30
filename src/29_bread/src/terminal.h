#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/stdint.h"

// Initialize the terminal
void terminal_initialize(void);

// Set the cursor position
void terminal_set_cursor_position(uint16_t row, uint16_t col);

// Other terminal functions
// ...existing code...

#endif // TERMINAL_H