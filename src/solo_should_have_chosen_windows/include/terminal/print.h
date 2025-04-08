#ifndef PRINT_H
#define PRINT_H

#include "libc/stdint.h"
#include "libc/stdarg.h"

void printf(const char *format, ...);
void reset_cursor();

#endif // PRINT_H