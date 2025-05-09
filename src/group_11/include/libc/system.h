#pragma once

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include "libc/limits.h"
#include "libc/stdio.h"



#include "string.h"

#define EOF (-1)


// Custom Functions
void panic(const char* reason);
char* hex32_to_str(char buffer[], unsigned int val);
char* int32_to_str(char buffer[], int val);
void* malloc(size_t size);
void free(void* ptr);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* ptr, int value, size_t num);
int printf(const char* format, ...);


