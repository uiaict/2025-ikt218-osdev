#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "i386/interruptRegister.h"

typedef struct
{
    int x;
    int y;
} vector2D;

#define BUFFER_SIZE 255

static char key_buffer[BUFFER_SIZE];

void write_to_buffer(char c);

void EnableBufferTyping();
void DisableBufferTyping();
void EnableTyping();
void DisableTyping();
char scanCodeToASCII(unsigned char *scanCode);
void irq1_keyboard_handler(registers_t *regs, void *ctx);
int has_user_pressed_esc();

static vector2D arrowKeys2D;

#endif