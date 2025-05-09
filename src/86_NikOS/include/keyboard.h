#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/stdbool.h"

void keyboard_handler(void);
void keyboard_install(void);

void not_playing_snake(void);
bool is_playing_snake(void);

#endif
