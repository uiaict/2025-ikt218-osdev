#pragma once

#include "stdint.h"

int putchar(int ic);
int puts(const char* str);
bool print(const char* data, size_t length);
int printf(const char* __restrict__ format, ...);