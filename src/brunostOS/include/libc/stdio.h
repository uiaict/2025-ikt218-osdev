#pragma once

#include "stdint.h"
#include "stdbool.h"

void verify_cursor_pos();
void ctrlchar(int);

int putchar(const int);
void print(const unsigned char*);
// Only printf will automatically update cursor
int printf(const char* __restrict__ format, ...);