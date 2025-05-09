
#include <stddef.h> // ✅ For size_t

#include <stdbool.h>

#pragma once

int putchar(int ic);
bool print(const char* data, size_t length);
int printf(const char* __restrict__ format, ...);