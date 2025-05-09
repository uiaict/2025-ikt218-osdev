#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "irq.h"

void keyboard_install();
void keyboard_callback(struct registers regs);
char keyboard_getchar();
char keyboard_getchar_nb();
#endif