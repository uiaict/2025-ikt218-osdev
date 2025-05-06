// libc/keyboard.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/isr.h"    // for registers_t
#include <libc/stdint.h> // for uint8_t

// PS/2 scancodes for arrow keys (Set 1)
#define SCAN_CODE_KEY_UP     0x48
#define SCAN_CODE_KEY_DOWN   0x50
#define SCAN_CODE_KEY_LEFT   0x4B
#define SCAN_CODE_KEY_RIGHT  0x4D

// I/O ports
#define KEYBOARD_DATA_PORT   0x60
#define KEYBOARD_STATUS_PORT 0x64

// Holds the last key pressed (ASCII or mapped WASD/arrows)
extern volatile char last_key;

// Install IRQ1 handler; must be called before enabling interrupts
void initKeyboard(void);

// Returns & clears last_key; 0 if none
char get_last_key(void);

#endif // KEYBOARD_H
