#pragma once
#include <stdbool.h>
#include <libc/stddef.h>

////////////////////////////////////////
// Character and String Output
////////////////////////////////////////

// Write a single character to terminal
int putchar(int ic);

// Write raw data to terminal (returns success)
bool print(const char* data, size_t length);

// Formatted output (printf-style)
int printf(const char* __restrict__ format, ...);

////////////////////////////////////////
// Panic Handler
////////////////////////////////////////

// Display message and halt system
void panic(const char* message);
