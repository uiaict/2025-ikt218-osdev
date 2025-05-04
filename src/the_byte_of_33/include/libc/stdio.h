#pragma once

#include <libc/stdbool.h>
#include <libc/stddef.h>
#include <libc/stdarg.h> // For va_list support
#include <libc/stdint.h>

int putchar(int ic);
bool print(const char* data, size_t length);
void printf(const char* fmt, ...); // Variadic printf
void print_dec(uint32_t value);
void print_hex(uint32_t value);