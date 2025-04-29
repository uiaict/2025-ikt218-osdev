#pragma once
#include <stdint.h>
void restore_keyboard_handler();

void keyboard_init();
void keyboard_handler(uint8_t scancode);
void reset_input_buffer();
