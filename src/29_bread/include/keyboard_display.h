#ifndef KEYBOARD_DISPLAY_H
#define KEYBOARD_DISPLAY_H

#include <libc/stdint.h>

void draw_keyboard(uint8_t row, uint8_t col);
void keyboard_display_key_press(char key);
void keyboard_display_key_release(char key);
void set_key_state(uint8_t key_index, uint8_t state);

#endif