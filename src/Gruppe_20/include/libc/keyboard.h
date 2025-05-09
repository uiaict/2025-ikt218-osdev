#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/isr.h"

// Keyboard interrupt handler
void keyboard_callback(registers_t regs);

#endif // KEYBOARD_H
