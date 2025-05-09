#ifndef _STDLIB_H
#define _STDLIB_H

#include <libc/stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Basic random number generator
static uint32_t next = 1;

static inline int rand() {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

static inline void srand(unsigned int seed) {
    next = seed;
}

// Memory allocation functions (declared but implemented elsewhere)
void* malloc(size_t size);
void free(void* ptr);
void* calloc(size_t num, size_t size);
void* realloc(void* ptr, size_t size);

// String conversion
int atoi(const char* str);
long atol(const char* str);

// Other standard functions
void abort(void);
void exit(int status);

#ifdef __cplusplus
}
#endif

#endif /* _STDLIB_H */