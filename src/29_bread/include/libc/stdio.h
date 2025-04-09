#pragma once
#include <libc/stdbool.h>
#include <libc/stdint.h>

void putchar(char c);
bool print(const char* data, size_t length);
int printf(const char* __restrict__ format, ...);