#pragma once

#include "i386/interruptRegister.h"
// #include <libc/stdint.h>
// #include <libc/stdbool.h>

// void init_keyboard();
// void keyBoard_handler();
char scanCodeToASCII(unsigned char *scanCode);
void irq1_keyboard_handler(registers_t *regs, void *ctx);
