#pragma once

//#include <libc/stdint.h>
//#include <libc/stdbool.h>

//void init_keyboard();
//void keyBoard_hanAdler();

#include "../src/arch/i386/interuptRegister.h"  


char scanCodeToASCII(unsigned char* scanCode);

void irq1_keyboard_handler(struct InterruptRegisters* regs, void* ctx);



