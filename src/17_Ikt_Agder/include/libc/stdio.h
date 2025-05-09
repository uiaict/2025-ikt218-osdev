#pragma once
#ifndef STDIO_H
#define STDIO_H

#include "libc/stddef.h"
#include "libc/stdbool.h"

int putchar(int c);
bool print(const char* data, size_t length);
int printf(const char* __restrict__ format, ...);
void clear_screen();

#endif