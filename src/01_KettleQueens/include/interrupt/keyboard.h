#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/stdint.h"
#include "interrupt/interrupts.h"

// Scancode to ASCII lookup table 
extern char scancode_ascii[128];

// Keyboard interrupt handler
void keyboard_handler(struct InterruptRegisters* regs);

#endif
