#pragma once
#include "stdbool.h"
#include "stdint.h"

// Writes a single character to the screen
int putchar(int c);

// Prints a formatted string (supports %s, %d, %u, %x, %%)
int printf(const char* format, ...);
