#ifndef MEMORY_H
#define MEMORY_H

#include "libc/stdint.h"
#include "libc/stddef.h"

// Memory management functions
void init_kernel_memory(uint32_t* kernel_end);
void init_paging(void);
void print_memory_layout(void);
void* malloc(size_t size);
void free(void* ptr);

#endif /* MEMORY_H */