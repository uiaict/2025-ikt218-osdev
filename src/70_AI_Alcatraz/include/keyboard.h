#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/stdint.h"
#include "libc/stdbool.h"

// Keyboard buffer functions
char* get_keyboard_buffer();
void clear_keyboard_buffer();
bool is_key_pressed(uint8_t scancode);
void handle_backspace();

// Special key scancodes
#define KEY_ESC         0x01
#define KEY_1           0x02
#define KEY_ENTER       0x1C
#define KEY_SHIFT_L     0x2A
#define KEY_SHIFT_R     0x36
#define KEY_CTRL        0x1D
#define KEY_ALT         0x38
#define KEY_SPACE       0x39
#define KEY_CAPS_LOCK   0x3A
#define KEY_BACKSPACE   0x0E

#endif // KEYBOARD_H
