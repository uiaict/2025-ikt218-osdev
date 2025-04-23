
#ifndef PMALLOC_H
#define PMALLOC_H

#include "libc/stdint.h"
#include "libc/stddef.h"

void init_kernel_memory(void* kernel_end);
char* pmalloc(size_t size); /* Allocates memory of given size with page alignment */
void pfree(void *mem);
void print_memory_layout();

#endif

