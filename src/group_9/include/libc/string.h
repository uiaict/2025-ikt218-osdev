#pragma once
#include <stddef.h> 

size_t strlen(const char* str);
char* itoa(int value, char* buffer, int base);
void* memmove(void* dest, const void* src, size_t n);
void* memset(void* dest, int val, size_t len);
