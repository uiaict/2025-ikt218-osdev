#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <libc/stdint.h>
#include <libc/isr.h> // For typedef registers_t

void keyboard_init();
void keyboard_handler(registers_t regs);

#endif
