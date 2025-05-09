#include <libc/stdbool.h>
#pragma once

int putchar(int ic);
bool print(const char* data, size_t length);
// int printf(int colour, const char* __restrict__ format, ...);
void printf(int colour, const char* s, ...);