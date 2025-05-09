#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "libc/stdint.h"

/**
 * Initialize the kernel memory manager
 * 
 * @param addr Pointer to the end of the kernel in memory
 */
void init_kernel_memory(uint32_t* addr);

/**
 * Allocate memory from the kernel heap
 * 
 * @param size Size of memory to allocate
 * @return Pointer to allocated memory, or NULL if failed
 */
void* malloc(long unsigned int size);

/**
 * Free previously allocated memory
 * 
 * @param ptr Pointer to memory to free
 */
void free(void* ptr);

/**
 * Initialize paging
 */
void init_paging(void);

/**
 * Print the current memory layout
 */
void print_memory_layout(void);

#endif /* MEMORY_MANAGER_H */ 