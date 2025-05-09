#ifndef MEMUTILS_H
#define MEMUTILS_H

#include "libc/stdint.h"

void* memcpy(void*, const void*, size_t);
void* memset16 (void*, uint16_t, size_t); // Function to set a block of memory with a 16-bit value
void* memset (void*, uint8_t, size_t); // Function to set a block of memory with a byte value

#endif // MEMUTILS_H