#ifndef MEMORY_H
#define MEMORY_H

#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

// Memory block structure (used for allocation tracking)
typedef struct mem_block {
    size_t size;
    bool free;
    struct mem_block *next;
} mem_block_t;

// Initialize the kernel memory manager
void init_kernel_memory(unsigned long *start_addr);

// Print memory layout information
void print_memory_layout(void);

// Memory allocation functions
void *malloc(size_t size);
void free(void *ptr);

// Paging functions
void init_paging(void);

// Memory statistics
size_t get_total_memory(void);
size_t get_used_memory(void);
size_t get_free_memory(void);

#endif /* MEMORY_H */