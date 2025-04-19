//#include <libc/stddef.h>
#include <libc/stdbool.h>
//#include <libc/stdint.h>


#pragma once

int putchar(int ic);   
bool print(const char* data, size_t length);
int printf(const char* __restrict__ format, ...);

//trenger ikke dem, siden de er definert i print.c / slik at printf() fungerer