// keyboard.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <libc/stdint.h>

// Special key scancodes
#define SCANCODE_ESCAPE    0x01
#define SCANCODE_BACKSPACE 0x0E
#define SCANCODE_TAB       0x0F
#define SCANCODE_ENTER     0x1C
#define SCANCODE_LCTRL     0x1D
#define SCANCODE_LSHIFT    0x2A
#define SCANCODE_RSHIFT    0x36
#define SCANCODE_LALT      0x38
#define SCANCODE_CAPSLOCK  0x3A
#define SCANCODE_F1        0x3B
#define SCANCODE_F2        0x3C
#define SCANCODE_F3        0x3D
#define SCANCODE_F4        0x3E
#define SCANCODE_F5        0x3F
#define SCANCODE_F6        0x40
#define SCANCODE_F7        0x41
#define SCANCODE_F8        0x42
#define SCANCODE_F9        0x43
#define SCANCODE_F10       0x44
#define SCANCODE_F11       0x57
#define SCANCODE_F12       0x58
#define SCANCODE_UP        0x48
#define SCANCODE_LEFT      0x4B
#define SCANCODE_RIGHT     0x4D
#define SCANCODE_DOWN      0x50

// Keyboard buffer size
#define KEYBOARD_BUFFER_SIZE 256

// Initialize the keyboard
void keyboard_init();

// Keyboard interrupt handler
void keyboard_handler(uint8_t interrupt_num);

// Get the current keyboard buffer
const char* get_keyboard_buffer();

// Check if the keyboard buffer is empty
int keyboard_buffer_empty();

// Process keyboard input (called from your main loop)
void process_keyboard_input();

#endif /* KEYBOARD_H */