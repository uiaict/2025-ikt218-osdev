#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"
#include "isr.h"

// Key codes for special keys
#define KEY_ESCAPE    1
#define KEY_BACKSPACE 14
#define KEY_TAB       15
#define KEY_ENTER     28
#define KEY_CTRL      29
#define KEY_LSHIFT    42
#define KEY_RSHIFT    54
#define KEY_ALT       56
#define KEY_CAPSLOCK  58
#define KEY_F1        59
#define KEY_F2        60
#define KEY_F3        61
#define KEY_F4        62
#define KEY_F5        63
#define KEY_F6        64
#define KEY_F7        65
#define KEY_F8        66
#define KEY_F9        67
#define KEY_F10       68
#define KEY_F11       87
#define KEY_F12       88

// Initialize the keyboard
void init_keyboard();

// Check if shift is currently pressed
u8int keyboard_is_shift_pressed();

// Get the character for a given scancode (0 if not a printable character)
char keyboard_scancode_to_char(u8int scancode, u8int shift_state);

#endif