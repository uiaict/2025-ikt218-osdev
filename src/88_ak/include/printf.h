#ifndef PRINTF_H
#define PRINTF_H

#include "libc/system.h"
#include "monitor.h"

void Print(const char *format, ...);
void int_to_string(int num, char *str, int base);
void putc(char c);

#endif