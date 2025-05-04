#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <libc/stdint.h>
#include "interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif

void keyboard_handler(registers_t* r);
char keyboard_get_last_char(void);
void keyboard_clear_last_char(void); // Better keypress handling

#ifdef __cplusplus
}
#endif

#endif