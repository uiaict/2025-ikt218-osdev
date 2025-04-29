#pragma once
#include "stdint.h"
#include "stdbool.h"

int putchar(int ic);
bool print(const char* data, size_t length);
int printf(const char* __restrict__ format, ...);