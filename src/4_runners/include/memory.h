#pragma once

#include "libc/stdint.h"
#include "libc/stddef.h"

/**
 * Initializes a simple heap from 0x400000 to 0x800000.
 */
void init_kernel_memory(uint32_t* kernel_end);

/**
 * Enables paging by:
 *   - Creating a page directory at 0x300000,
 *   - Creating a page table at 0x301000,.
 */
void init_paging(void);

/**
 * Allocates ‘size’ bytes from the kernel heap. Returns pointer on success,
 * or NULL if there’s not enough space.
 */
void* malloc(size_t size);

/**
 * Frees a previously allocated block. If ptr is NULL, does nothing.
 */
void free(void* ptr);

/**
 * Prints the memory layout of the kernel heap.
 * This is useful for debugging and understanding memory usage.
 */
void print_memory_layout(void);

void print_hex(uint32_t value); // forward declaration



