#ifndef KEYBOARD_MAP_H
#define KEYBOARD_MAP_H

#include "libc/stdint.h"

/* This keyboard map is for a norwegian keyboard layout */

// Define modifier keys
#define KEYBOARD_LSHIFT 0x2A
#define KEYBOARD_RSHIFT 0x36
#define KEYBOARD_ALT_GR 0x38
#define KEYBOARD_SIZE 128


// Lookup tables for Norwegian QWERTY
extern const unsigned char keyboard_normal[KEYBOARD_SIZE];
extern const unsigned char keyboard_shift[KEYBOARD_SIZE];
extern const unsigned char keyboard_altgr[KEYBOARD_SIZE];

#endif