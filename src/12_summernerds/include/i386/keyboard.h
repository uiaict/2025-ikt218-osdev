#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "i386/interruptRegister.h"
// #include <libc/stdint.h>
// #include <libc/stdbool.h>

typedef struct{
    int x;
    int y;
}vector2D __attribute__((packed));

void EnableTyping();
void DisableTyping();
char scanCodeToASCII(unsigned char *scanCode);
void irq1_keyboard_handler(registers_t *regs, void *ctx);

static vector2D arrowKeys2D;

#endif