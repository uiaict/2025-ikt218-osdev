#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <libc/stdint.h>
#include "interrupt.h"

void init_keyboard_controller(void);
void keyboard_handler(registers_t *r);
char keyboard_get_last_char(void);
void keyboard_clear_last_char(void);

#endif
