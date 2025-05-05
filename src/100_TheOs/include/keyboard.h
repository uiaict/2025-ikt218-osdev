#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "interrupts.h"
#include "libc/stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CHAR_NONE 0
#define CHAR_ENTER 2
#define CHAR_SPACE 3
#define CHAR_BACKSPACE 8

void start_keyboard(void);
void keyboard_controller(registers_t* regs, void* context);
char scancode_to_ascii(uint8_t scancode);

#ifdef __cplusplus
}
#endif

#endif // KEYBOARD_H
