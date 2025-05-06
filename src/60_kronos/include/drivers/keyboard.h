#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/stdint.h"
#include "kernel/isr.h"

// Constants for keyboard ports
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_COMMAND_PORT 0x64

// Buffer size for keyboard input
#define KEYBOARD_BUFFER_SIZE 256

// Special keys
#define KEY_BACKSPACE 0x0E
#define KEY_ENTER 0x1C
#define KEY_LEFT_SHIFT 0x2A
#define KEY_RIGHT_SHIFT 0x36
#define KEY_LEFT_SHIFT_RELEASE 0xAA
#define KEY_RIGHT_SHIFT_RELEASE 0xB6
#define KEY_CAPS_LOCK 0x3A

// Function prototypes
void keyboard_init();
void keyboard_handler(registers_t regs);
char keyboard_get_last_char();
void keyboard_buffer_clear();
int is_key_pressed(char key);

#endif // KEYBOARD_H