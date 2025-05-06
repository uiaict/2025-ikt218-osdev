#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "keyboard.h"

void verify_cursor_pos();
void ctrlchar(int);

int putchar(const int);
void print(const unsigned char*);
// Only printf will automatically update cursor
int printf(const char* __restrict__ format, ...);

int getchar();
void scanf(unsigned char* __restrict__ format, ...);
extern volatile unsigned char buffer[256];
extern int buffer_index;