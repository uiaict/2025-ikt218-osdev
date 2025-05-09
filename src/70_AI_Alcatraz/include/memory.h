#ifndef MEMORY_H
#define MEMORY_H

#include "libc/stdint.h"
#include "libc/stddef.h"

// Memory management functions
void init_kernel_memory(uint32_t* start_address);
void init_paging();
void print_memory_layout();

// Standard memory allocation functions
void* malloc(size_t size);
void free(void* ptr);

// Memory block structure for allocation tracking
typedef struct memory_block {
    size_t size;
    uint8_t is_free;
    struct memory_block* next;
} memory_block_t;

#endif // MEMORY_H
