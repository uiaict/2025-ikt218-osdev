#pragma once

#include "stddef.h"  // for size_t 
#include "stdarg.h"  // for va_list

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

/**
 * Copies the string src to dest, including the terminating null byte.
 * Returns a pointer to the destination string dest.
 */
char* strcpy(char* dest, const char* src);

/**
 * Appends the src string to the dest string, overwriting the terminating null byte.
 * Returns a pointer to the resulting string dest.
 */
char* strcat(char* dest, const char* src);

/**
 * Writes the formatted string to the buffer str.
 * Format specifiers: %d for integers.
 * Returns the number of characters written.
 */
int sprintf(char* str, const char* format, ...);