#pragma once

#include "libc/stdbool.h"
#include "libc/stddef.h"

int putchar(int ic);
//bool print(const char* data, size_t length);
void printf(const char* __restrict__ format, ...);