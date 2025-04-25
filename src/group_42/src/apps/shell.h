#ifndef SHELL_H
#define SHELL_H

#include "libc/stdbool.h"

extern bool shell_active;

void shell_init();
void shell_input(char character);

#endif // SHELL_H