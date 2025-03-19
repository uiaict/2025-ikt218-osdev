#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stdint.h"
#include "types.h"  // for struct InterruptRegisters

// Define special key constant (others can be defined in keyboard.c if desired)
extern const uint32_t CAPS;

// Declare global flags for modifier keys
extern bool capsOn;
extern bool capsLock;

void keyboardHandler(struct InterruptRegisters *regs);
void initKeyboard(void);

#endif
