#pragma once
#include "libc/stddef.h"

// skriver én karakter til VGA‐buffer
int putchar(int c);

// skriver en buffer av fixed length
bool print(const char* data, size_t length);

// standard printf
int printf(const char* __restrict__ format, ...);
