#ifndef MEMORY_H
#define MEMORY_H

#include "libc/stdint.h"
#include "libc/stdbool.h"
//
// Definition of a struct that represents a memory allocation.
// It contains a status field (0 or 1) indicating if the memory
// is currently allocated or not, and a size field indicating
// the size of the allocated memory in bytes.
//
typedef struct {
    uint8_t status;
    uint32_t size;
} alloc_t;

static bool do_print = false;

void init_kernel_memory(uint32_t*);
void print_when_allocating(bool);
void print_memory_layout();

/* Function declarations for memory allocation */
void* malloc(size_t); /* Allocates memory of given size */
void free(void*); /* Frees memory previously allocated */
char* pmalloc(size_t); /* Allocates memory of given size with page alignment */
void pfree(void*); /* Frees memory previously allocated with page alignment */

#endif // MEMORY_H