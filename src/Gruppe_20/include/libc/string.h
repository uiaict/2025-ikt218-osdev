#ifndef STRING_H
#define STRING_H

#include "libc/stddef.h"  // For size_t

size_t strlen(const char* str);
void* memset(void* ptr, int value, size_t num);
void* memcpy(void* destination, const void* source, size_t num);
int memcmp(const void* ptr1, const void* ptr2, size_t num);
char* strcpy(char* destination, const char* source);
int strcmp(const char* str1, const char* str2);

#endif // STRING_H