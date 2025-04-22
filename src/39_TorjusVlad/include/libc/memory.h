#ifndef MEMORY_H
#define MEMORY_H

#include "libc/stdint.h"
#include "libc/stddef.h"

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
void* memmove(void* dest, const void* src, size_t n);

void heap_init (void* heap_mem_start, size_t heap_size);
void* malloc (size_t size);
void free (void* ptr);
void print_heap();

#endif // MEMORY_H