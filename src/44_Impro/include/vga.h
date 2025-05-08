#pragma once
#include "libc/stdint.h"

#define COLOR_BACK 0
#define COLOR_FRONT 7

#define width 80
#define height 25

void scrollUp();
void nl();
void clear();
int printf(const char* format, ...);
int putchar(int ch);
void print_hex(uint32_t num);