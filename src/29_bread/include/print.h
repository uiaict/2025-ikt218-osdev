#ifndef printf_h
#define printf_h

#include <libc/stdint.h>
#include "putchar.h"
#include "libc/stdarg.h"

// Only declare the functions here, don't define them
int print_string(const char* str);
int print_int(int num);
int print_hex(unsigned int num);  // Add this line
int printf(const char* format, ...);

#endif
