#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "i386/interruptRegister.h"

typedef struct
{
    int x;
    int y;
} vector2D;

void EnableBufferTyping();
void DisableBufferTyping();
void EnableTyping();
void DisableTyping();
char scanCodeToASCII(unsigned char *scanCode);
void irq1_keyboard_handler(registers_t *regs, void *ctx);

static vector2D arrowKeys2D;

#endif