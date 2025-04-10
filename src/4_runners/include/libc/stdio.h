#pragma once

#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/stdarg.h"

#define EOF (-1) // ← Add this line!

int putchar(int ic);
bool print(const char* data, size_t length);
int printf(const char* __restrict__ format, ...);
