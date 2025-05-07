#pragma once

#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>
#include "libc/stdbool.h"

char* strncpy(char* dest, const char* src, size_t n);
int32_t strncmp(const char* s1, const char* s2, size_t n);
char* strtok(char* str, const char* delim);
char* strchr(const char* str, int32_t c);
void* memset(void* dest, int32_t value, size_t count);
size_t strlen(const char* str);
bool strcontains(const char* str, const char delim);

#endif