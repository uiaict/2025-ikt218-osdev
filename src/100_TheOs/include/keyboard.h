#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "interrupts.h"

// Constants for keyboard input
#define CHAR_NONE 0
#define CHAR_ENTER 2
#define CHAR_SPACE 3
#define CHAR_BACKSPACE 8

// Start keyboard controller
// This function initializes the keyboard controller and registers the keyboard ISR
void start_keyboard(void);
// Keyboard controller function
// Reads the scancode from the keyboard and processes it
// Supports key press and release events
// Converts the scancode to ASCII and adds it to the command buffer
void keyboard_controller(registers_t* regs, void* context);
// Convert scancode to ASCII
// This function takes a scancode as input and returns the corresponding ASCII character
char scancode_to_ascii(unsigned char scancode);

#endif