#ifndef MEMORY_H
#define MEMORY_H

#include "libc/system.h"

// Struct to track a memory block's status and size
typedef struct {
    uint8_t status;
    uint32_t size;
} alloc_t;

// Initializes memory management after kernel
void init_kernel_memory(uint32_t* kernel_end);

// Paging functions
void init_paging();
void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys);

// Memory allocation
char* pmalloc(size_t size);
void* malloc(size_t size);
void free(void *mem);

// Memory operations
void* memcpy(void* dest, const void* src, size_t num);
void* memset(void *ptr, int value, size_t num);
void* memset16(void *ptr, uint16_t value, size_t num);

// Shows memory layout (for debugging)
void print_memory_layout();

#endif
