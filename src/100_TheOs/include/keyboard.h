#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "interrupts.h"

// Key event types
#define CHAR_NONE 0
#define CHAR_ENTER 2
#define CHAR_SPACE 3

// Function prototypes
void start_keyboard(void);
void keyboard_controller(registers_t* regs, void* context);
char scancode_to_ascii(unsigned char scancode);

#endif