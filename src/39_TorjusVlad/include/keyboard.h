#pragma once

#include "libc/stdint.h"
#include "libc/stdbool.h"

void init_keyboard();

void keyboard_handle_scancode(uint8_t scancode);
bool keyboard_has_char();
char keyboard_get_char();