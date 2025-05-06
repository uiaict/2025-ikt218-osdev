#pragma once
#include "i386/interruptRegister.h"


char scanCodeToASCII(unsigned char *scanCode);
void irq1_keyboard_handler(registers_t *regs, void *ctx);
