#pragma once
#include "libc/stdint.h"
#include "libc/stdbool.h"

size_t strlen(const char *str);
int strcmp(const char *str1, const char *str2);

void *memmove(void *dest, const void *src, size_t n);
char *strtok(char *str, const char *delim);