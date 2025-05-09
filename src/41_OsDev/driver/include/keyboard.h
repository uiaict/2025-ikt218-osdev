#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <libc/stdint.h>
#include <libc/stdbool.h>

////////////////////////////////////////
// Keyboard KeyCode Type
////////////////////////////////////////

typedef uint8_t KeyCode;

////////////////////////////////////////
// Special Key Definitions
////////////////////////////////////////

enum {
    KEY_UP      = 0x80,
    KEY_DOWN    = 0x81,
    KEY_LEFT    = 0x82,
    KEY_RIGHT   = 0x83,

    KEY_ENTER   = '\r',
    KEY_ESC     = 0x1B,
    KEY_SPACE   = ' '
};

////////////////////////////////////////
// Keyboard Interface
////////////////////////////////////////

// Initialize the keyboard driver
void     keyboard_initialize(void);

// Wait for a key press and return it
KeyCode  keyboard_get_key(void);

// Check if the keyboard buffer is empty
bool     keyboard_buffer_empty(void);

// Retrieve the next key from buffer (returns 0 if empty)
KeyCode  keyboard_buffer_dequeue(void);

#endif 