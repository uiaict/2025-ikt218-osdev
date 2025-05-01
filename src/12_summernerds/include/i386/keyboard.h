#pragma once
#include "i386/IRQ.h"

void irq1_keyboard_handler(registers_t* regs, void* ctx);
char scanCodeToASCII(unsigned char* scanCode);
