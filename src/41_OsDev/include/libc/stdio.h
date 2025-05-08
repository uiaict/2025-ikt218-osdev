#pragma once
#include <stdbool.h>

int putchar(int ic);
bool print(const char* data, size_t length);
int printf(const char* __restrict__ format, ...);
void panic(const char* message);  // Add this line