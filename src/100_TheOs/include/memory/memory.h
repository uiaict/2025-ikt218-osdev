#ifndef MEMORY_H
#define MEMORY_H

#include "libc/system.h"

#include "memory/memory.h" 
#include "libc/stdbool.h" 
#include "libc/stdint.h"
#include "libc/stddef.h"  // for size_t

uint32_t get_memory_used(void);


// Optional: allocation block metadata
typedef struct {
    uint8_t used;       // 0 = free, 1 = used
    uint32_t size;      // size in bytes
} memory_block_t;

// Memory system
void init_kernel_memory(uint32_t* kernel_end);
void print_memory_layout(void);

// Paging setup
void init_paging(void);
void paging_map_virtual_to_phys(uint32_t virtual_addr, uint32_t physical_addr);

// Basic alloc/free
void* malloc(size_t size);
void free(void* ptr);

// Optional: page-aligned malloc
void* pmalloc(size_t size);

// Memory helper functions
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* ptr, int value, size_t n);
void* memset16(void* ptr, uint16_t value, size_t n);
#endif
