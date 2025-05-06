#ifndef _STDLIB_H
#define _STDLIB_H

#include "libc/stddef.h"  // Include if size_t is needed
#include "libc/stdint.h"

// Memory management function declarations
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t new_size);

// Integer conversion
int atoi(const char *str);

// Exit function
void exit(int status);

#endif // _STDLIB_H
