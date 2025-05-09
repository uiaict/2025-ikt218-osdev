#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/stdint.h"

// Define the size of the keyboard buffer
#define KEYBOARD_BUFFER_SIZE 128

// Scancode to ASCII lookup table
extern char scancode_to_ascii[128];

// Keyboard buffer
extern char keyboard_buffer[KEYBOARD_BUFFER_SIZE];

// Function to handle IRQ1 (keyboard)
void irq1_handler();

// Function to initialize the keyboard
void keyboard_init();

#endif
