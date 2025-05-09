#pragma once

#include "libc/stddef.h"
#include "libc/stdint.h"


size_t strlen(const char* str);
void *memset(void *dest, int val, size_t len);
int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t n);
void strncpy(char *dest, const char *src, size_t n);