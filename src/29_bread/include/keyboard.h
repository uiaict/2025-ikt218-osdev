#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <libc/stdint.h>
#include <libc/isr.h>

// Function prototypes
void init_keyboard();
void keyboard_handler(registers_t regs);
char scancode_to_ascii(uint8_t scancode);
void keyboard_buffer_add(char c);
char keyboard_buffer_get();
uint8_t keyboard_buffer_size();
void play_key_note(char key);

#endif
