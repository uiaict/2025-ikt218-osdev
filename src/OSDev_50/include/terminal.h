#ifndef TERMINAL_H
#define TERMINAL_H

#include "libc/stddef.h"
#include "libc/stdint.h"

void terminal_initialize(void);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#endif