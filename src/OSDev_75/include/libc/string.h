#pragma once

#include "stddef.h"  // for size_t 

/**
 * Returns the length of a null-terminated string.
 */
size_t strlen(const char* str);

/**
 * Sets `count` bytes in `dest` to `value`.
 */
void* memset(void* dest, int value, size_t count);

/**
 * Converts an integer `value` to a null-terminated string using the specified `base`.
 * Returns `str`.
 */
char* itoa(int value, char* str, int base);
