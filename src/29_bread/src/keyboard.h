#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/stdint.h"
#include "libc/stdbool.h"

// Add this declaration for the function we created
void on_key_press(uint8_t scancode, bool is_pressed);

#endif // KEYBOARD_H