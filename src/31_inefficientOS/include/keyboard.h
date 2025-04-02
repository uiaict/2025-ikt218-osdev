#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/stdint.h"

// Initialize the keyboard driver
void keyboard_init();

// Get a character from the keyboard buffer (non-blocking)
// Returns 0 if no key is available
char keyboard_getchar();

// Get a scancode from the keyboard buffer (non-blocking)
// Returns 0 if no scancode is available
uint8_t keyboard_get_scancode();

// Convert a scancode to an ASCII character
char keyboard_scancode_to_ascii(uint8_t scancode);

#endif // KEYBOARD_H