#pragma once
#include "libc/stdint.h"

// Returns the length of a null-terminated string
size_t strlen(char *str);

// Returns a pointer to the first occurrence of character 'c' in the string
char *strchr(char *str, int c);

// Reverses a string in place
void strrev(char str[], int length);

// Compares two strings; returns 0 if equal, negative if str1 < str2, positive if str1 > str2
int strcmp(const char *str1, const char *str2);
