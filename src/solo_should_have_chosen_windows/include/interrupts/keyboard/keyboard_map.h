#ifndef KEYBOARD_MAP_H
#define KEYBOARD_MAP_H

#include "libc/stdint.h"

/* This keyboard map is for a norwegian keyboard layout */

// Define modifier keys
#define KEYBOARD_LSHIFT 0x2A
#define KEYBOARD_RSHIFT 0x36
#define KEYBOARD_ALT_GR 0x38

// Lookup tables for Norwegian QWERTY
extern const char keyboard_normal[128];
extern const char keyboard_shift[128];
extern const char keyboard_altgr[128];

#endif