#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdio.h>

void init_kernel_memory(void *kernel_end);
void *malloc(size_t size);
void free(void *ptr);

void *operator_new(size_t size);
void operator_delete(void *ptr);

uintptr_t get_kernel_heap_start(void);
uintptr_t get_kernel_heap_end(void);

void print_heap_blocks(void);
void print_memory_layout(void);

#endif
