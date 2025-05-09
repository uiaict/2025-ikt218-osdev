#ifndef LIBC_SYSTEM_H
#define LIBC_SYSTEM_H

#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdbool.h>

// Basic printf-like function for kernel debugging
void printf(const char* format, ...);

// Function to halt the system in case of unrecoverable errors
void panic(const char* message);

// Memory management functions
// These are already defined in memory.h but are used in system.h
void* memcpy(void* dest, const void* src, size_t count);
void* memset(void* ptr, int value, size_t num);
void* memset16(void* ptr, uint16_t value, size_t num);

#endif // LIBC_SYSTEM_H